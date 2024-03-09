#include "mxIgraph.h"
#include "speak_easy_2.h"

#include <igraph_interface.h>
#include <string.h>

#define STREQ(a, b) strcmp((a), (b)) == 0

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  igraph_t graph;
  igraph_vector_t weights;
  mxArray const* method_options = prhs[1];
  igraph_vector_int_t membership;

  igraph_bool_t isdirected = mxIgraphGetBool(method_options, "isdirected") ?
                             IGRAPH_DIRECTED : IGRAPH_UNDIRECTED;
  se2_options opts = {
    .discard_transient = mxIgraphGetInteger(method_options, "discardTransient"),
    .independent_runs = mxIgraphGetInteger(method_options, "independentRuns"),
    .max_threads = mxIgraphGetInteger(method_options, "maxThreads"),
    .minclust = mxIgraphGetInteger(method_options, "minClusters"),
    .node_confidence = mxIgraphGetBool(method_options, "nodeConfidence"),
    .random_seed = mxIgraphGetInteger(method_options, "seed"),
    .subcluster = mxIgraphGetInteger(method_options, "subcluster"),
    .target_clusters = mxIgraphGetInteger(method_options, "targetClusters"),
    .target_partitions = mxIgraphGetInteger(method_options, "targetPartitions"),
    .verbose = mxIgraphGetBool(method_options, "verbose")
  };

  mxIgraphGetGraph(prhs[0], &graph, &weights, isdirected);
  igraph_vector_int_init(&membership, 0);

  speak_easy_2(&graph, &weights, &opts, &membership);

  igraph_destroy(&graph);
  igraph_vector_destroy(&weights);

  plhs[0] = mxIgraphMembershipToArray(&membership);
  igraph_vector_int_destroy(&membership);
}
