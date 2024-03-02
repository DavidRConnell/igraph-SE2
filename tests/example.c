#include "igraph.h"
#include "speak_easy_2.h"

int main()
{
  igraph_t graph;
  igraph_famous(&graph, "Zachary");
  igraph_vector_int_t membership;
  se2_options opts = {};

  igraph_vector_int_init(&membership, 0);
  speak_easy_2(&graph, NULL, &opts, &membership);

  igraph_destroy(&graph);

  printf("[");
  for (igraph_integer_t i = 0; i < igraph_vector_int_size(&membership); i++) {
    printf(" %"IGRAPH_PRId, VECTOR(membership)[i]);
  }
  printf(" ]\n");

  igraph_vector_int_destroy(&membership);

  return EXIT_SUCCESS;
}
