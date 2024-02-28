#ifndef SE2_PRINT_H
#define SE2_PRINT_H

#include <igraph_foreign.h>
#include <igraph_community.h>
#include "se2_partitions.h"

#ifndef SE2_PRINT_PATH
#error Must define SE2_PRINT_PATH if printing
#endif

#define STR_VALUE(p) #p
#define PATH(dir, base) STR_VALUE(dir) STR_VALUE(/) STR_VALUE(base)

#define EDGE_PATH PATH(SE2_PRINT_PATH, edges.txt)
#define STAGES_PATH PATH(SE2_PRINT_PATH, stages.tsv)
#define MEMBERSHIP_PATH PATH(SE2_PRINT_PATH, membership.tsv)

void se2_print_step(se2_partition const *partition,
                    igraph_integer_t const time, igraph_integer_t const mode,
                    igraph_real_t const *modularity)
{
  FILE *stages = fopen(STAGES_PATH, "a");
  FILE *membership = fopen(MEMBERSHIP_PATH, "a");

  fprintf(stages, "%"IGRAPH_PRId"\t%"IGRAPH_PRId"\t%f\n", time,
          mode, *modularity);

  fprintf(membership, "%"IGRAPH_PRId, time);
  for (igraph_integer_t i = 0; i < partition->n_nodes; i++) {
    fprintf(membership, "\t%"IGRAPH_PRId, LABEL(*partition)[i]);
  }
  fprintf(membership, "\n");

  fclose(stages);
  fclose(membership);
}

void se2_print_setup(igraph_t const *graph, se2_partition const *partition,
                     igraph_real_t const *modularity)
{
  FILE *fptr;
  if (!(fptr = fopen(EDGE_PATH, "r"))) {
    fptr = fopen(EDGE_PATH, "w");
    fprintf(fptr, "from to\n");
    igraph_write_graph_edgelist(graph, fptr);
  }
  fclose(fptr);

  FILE *stages = fopen(STAGES_PATH, "w");
  FILE *membership = fopen(MEMBERSHIP_PATH, "w");

  fprintf(stages, "timestep\tstage\tmodularity\n");

  fprintf(membership, "timestep");
  for (igraph_integer_t i = 0; i < partition->n_nodes; i++) {
    fprintf(membership, "\tn%"IGRAPH_PRId, i);
  }
  fprintf(membership, "\n");

  fclose(stages);
  fclose(membership);

  se2_print_step(partition, 0, 0, modularity);
}
#endif
