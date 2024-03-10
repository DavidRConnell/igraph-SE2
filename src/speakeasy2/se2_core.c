#include <igraph_error.h>
#include <igraph_structural.h>
#include <igraph_community.h>
#include <omp.h>

#include "speak_easy_2.h"
#include "se2_seeding.h"
#include "se2_random.h"
#include "se2_modes.h"
#include "se2_reweight_graph.h"

#define SE2_SET_OPTION(opts, field, default) \
    (opts->field) = (opts)->field ? (opts)->field : (default)

static void se2_core(igraph_t const* graph,
                     igraph_vector_t const* weights,
                     igraph_vector_int_list_t* partition_list,
                     igraph_integer_t const partition_offset,
                     se2_options const* opts)
{
  se2_tracker* tracker = se2_tracker_init(opts);
  igraph_vector_int_t* ic_store =
    igraph_vector_int_list_get_ptr(partition_list, partition_offset);
  se2_partition* working_partition = se2_partition_init(graph, ic_store);

  igraph_integer_t partition_idx = partition_offset;
  for (igraph_integer_t time = 0; !se2_do_terminate(tracker); time++) {
    se2_mode_run_step(graph, weights, working_partition, tracker, time);
    if (se2_do_save_partition(tracker)) {
      se2_partition_store(working_partition, partition_list, partition_idx);
      partition_idx++;
    }
  }

  se2_tracker_destroy(tracker);
  se2_partition_destroy(working_partition);
  return;
}

static void se2_most_representative_partition(igraph_vector_int_list_t const
    *partition_store, igraph_integer_t const n_partitions,
    igraph_vector_int_t* most_representative_partition,
    se2_options const* opts)
{
  igraph_vector_int_t* selected_partition;
  igraph_matrix_t nmi_sum_accumulator;
  igraph_vector_t nmi_sums;
  igraph_integer_t idx = 0;
  igraph_real_t max_nmi = -1;
  igraph_real_t mean_nmi = 0;

  igraph_matrix_init(&nmi_sum_accumulator, n_partitions, opts->max_threads);
  igraph_vector_init(&nmi_sums, n_partitions);

  #pragma omp parallel for
  for (igraph_integer_t i = 0; i < n_partitions; i++) {
    igraph_real_t nmi;
    igraph_integer_t thread_i = omp_get_thread_num();
    for (igraph_integer_t j = (i + 1); j < n_partitions; j++) {
      igraph_compare_communities(
        igraph_vector_int_list_get_ptr(partition_store, i),
        igraph_vector_int_list_get_ptr(partition_store, j),
        &nmi,
        IGRAPH_COMMCMP_NMI);
      MATRIX(nmi_sum_accumulator, i, thread_i) += nmi;
      MATRIX(nmi_sum_accumulator, j, thread_i) += nmi;
    }
  }

  igraph_matrix_rowsum(&nmi_sum_accumulator, &nmi_sums);

  if (opts->verbose) {
    mean_nmi = igraph_matrix_sum(&nmi_sum_accumulator);
    mean_nmi /= (n_partitions * (n_partitions - 1)) / 2;
    printf("mean of all NMIs %0.5f\n", mean_nmi);
  }

  for (igraph_integer_t i = 0; i < n_partitions; i++) {
    if (VECTOR(nmi_sums)[i] > max_nmi) {
      max_nmi = VECTOR(nmi_sums)[i];
      idx = i;
    }
  }

  igraph_matrix_destroy(&nmi_sum_accumulator);
  igraph_vector_destroy(&nmi_sums);

  selected_partition = igraph_vector_int_list_get_ptr(partition_store, idx);

  igraph_integer_t n_nodes = igraph_vector_int_size(selected_partition);
  igraph_vector_int_init(most_representative_partition, n_nodes);
  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    VECTOR(*most_representative_partition)[i] = VECTOR(*selected_partition)[i];
  }
}

static void se2_bootstrap(igraph_t* graph,
                          igraph_vector_t const* weights,
                          igraph_integer_t const subcluster_iter,
                          se2_options const* opts,
                          igraph_vector_int_t* res)
{
  igraph_integer_t n_nodes = igraph_vcount(graph);
  igraph_vector_t kin;
  igraph_integer_t n_partitions = opts->target_partitions *
                                  opts->independent_runs;
  igraph_vector_int_list_t partition_store;

  igraph_vector_init(&kin, n_nodes);
  igraph_strength(graph, &kin, igraph_vss_all(), IGRAPH_IN, IGRAPH_LOOPS,
                  weights);

  igraph_vector_int_list_init(&partition_store, n_partitions);

  if ((opts->verbose) && (!subcluster_iter) && (opts->multicommunity > 1)) {
    puts("attempting overlapping clustering");
  }

  #pragma omp parallel for
  for (igraph_integer_t run_i = 0; run_i < opts->independent_runs; run_i++) {
    igraph_integer_t partition_offset = run_i * opts->target_partitions;
    igraph_vector_int_t ic_store;
    igraph_vector_int_init(&ic_store, n_nodes);

    se2_rng_init(run_i + opts->random_seed);
    igraph_integer_t n_unique = se2_seeding(graph, weights, &kin, opts,
                                            &ic_store);
    igraph_vector_int_list_set(&partition_store, partition_offset, &ic_store);

    if ((opts->verbose) && (!subcluster_iter) && (run_i == 0)) {
      printf("completed generating initial labels\n"
             "produced about %"IGRAPH_PRId" seed labels, "
             "while goal was %"IGRAPH_PRId"\n"
             "starting level 1 clustering; "
             "independent runs might not be displayed in order - "
             "that is okay\n",
             n_unique, opts->target_clusters);
    }

    if ((opts->verbose) && (!subcluster_iter)) {
      printf("\nstarting independent run #%"IGRAPH_PRId" of %"IGRAPH_PRId"\n",
             run_i + 1, opts->independent_runs);
    }

    se2_core(graph, weights, &partition_store, partition_offset, opts);
  }

  se2_most_representative_partition(&partition_store,
                                    n_partitions,
                                    res, opts);

  if ((opts->verbose) && (!subcluster_iter)) {
    printf("generated %"IGRAPH_PRId" partitions at level 1\n", n_partitions);
  }

  igraph_vector_int_list_destroy(&partition_store);

  igraph_vector_destroy(&kin);
}

static igraph_integer_t default_target_clusters(igraph_t const* graph)
{
  igraph_integer_t n_nodes = igraph_vcount(graph);

  if (n_nodes < 10) {
    return n_nodes;
  }

  if ((n_nodes / 100) < 10) {
    return 10;
  }

  return n_nodes / 100;
}

static igraph_integer_t default_max_threads()
{
  igraph_integer_t n_threads = 0;
  // Hack since omp_get_num_threads returns 1 outside of a parallel block
  #pragma omp parallel
  {
    #pragma omp single
    n_threads = omp_get_num_threads();
  }
  return n_threads;
}

static void se2_set_defaults(igraph_t const* graph, se2_options* opts)
{
  SE2_SET_OPTION(opts, independent_runs, 10);
  SE2_SET_OPTION(opts, subcluster, 1);
  SE2_SET_OPTION(opts, multicommunity, 1);
  SE2_SET_OPTION(opts, target_partitions, 5);
  SE2_SET_OPTION(opts, target_clusters, default_target_clusters(graph));
  SE2_SET_OPTION(opts, minclust, 5);
  SE2_SET_OPTION(opts, discard_transient, 3);
  SE2_SET_OPTION(opts, random_seed, RNG_INTEGER(1, 9999));
  SE2_SET_OPTION(opts, max_threads, default_max_threads());
  SE2_SET_OPTION(opts, node_confidence, false);
  SE2_SET_OPTION(opts, verbose, false);

  omp_set_num_threads(opts->max_threads);
}

int speak_easy_2(igraph_t* graph, igraph_vector_t* weights,
                 se2_options* opts, igraph_vector_int_t* res)
{
  se2_set_defaults(graph, opts);

  if (opts->verbose) {
    igraph_integer_t possible_edges = igraph_vcount(graph);
    possible_edges *= possible_edges;
    igraph_real_t edge_density = (igraph_real_t)igraph_ecount(graph) /
                                 possible_edges;
    igraph_bool_t directed = igraph_is_directed(graph);
    edge_density *= (!directed + 1);
    printf("approximate edge density is %0.5f\n"
           "input type treated as %s\n"
           "ADJ is %s\n"
           "calling main routine at level 1\n",
           edge_density, weights ? "weighted" : "unweighted",
           directed ? "asymmetric" : "symmetric");
  }

  igraph_bool_t isweighted = weights ? true : false;
  igraph_vector_t weights_i;
  if (isweighted) {
    weights_i = *weights;
  } else {
    igraph_vector_init(&weights_i, igraph_ecount(graph));
    for (igraph_integer_t i = 0; i < igraph_ecount(graph); i++) {
      VECTOR(weights_i)[i] = 1;
    }
  }

  se2_reweight(graph, &weights_i);

  se2_bootstrap(graph, &weights_i, 0, opts, res);

  /* if (opts->node_confidence) { */
  /*   // pass; */
  /* } */

  /* for (igraph_integer_t i = 1; i < opts->subcluster; i++) { */
  // pass;
  /* } */

  if (!isweighted) {
    igraph_vector_destroy(&weights_i);
  }

  return IGRAPH_SUCCESS;
}
