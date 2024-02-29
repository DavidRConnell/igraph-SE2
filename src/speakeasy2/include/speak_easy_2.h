#ifndef SPEAK_EASY_H
#define SPEAK_EASY_H

#include <igraph_datatype.h>

typedef struct {
  igraph_integer_t independent_runs;  // Number of independent runs to perform.
  igraph_integer_t subcluster;        // Depth of clustering.
  igraph_integer_t
  multicommunity;    // Max number of communities a node can be a member of.
  igraph_integer_t
  target_partitions; // Number of partitions to find per independent run.
  igraph_integer_t target_clusters;   // Expected number of clusters to find.
  igraph_integer_t minclust;          // Minimum cluster size to subclustering.
  igraph_integer_t
  discard_transient; // How many initial partitions to discard before recording.
  igraph_integer_t random_seed;       // Seed for reproducing results.
  igraph_integer_t max_threads;       // Number of threads to use.
  bool node_confidence;
  bool verbose; // Print information to stdout
} options;

int speak_easy_2(igraph_t *graph, igraph_vector_t *weights,
                 options *opts, igraph_vector_int_t *res);

#endif
