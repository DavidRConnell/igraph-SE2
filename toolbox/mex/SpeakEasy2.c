#include "mxIgraph.h"
#include "speak_easy_2.h"

#include <igraph_interface.h>
#include <string.h>

#define STREQ(a, b) strcmp((a), (b)) == 0

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  int n_required_inputs = 1;

  if (nrhs < n_required_inputs) {
    mexErrMsgIdAndTxt("SE2:WrongNumberOfInputs",
                      "%s requires atleast an adjacency matrix",
                      mexFunctionName());
  }

  if (mxIsComplex(prhs[0])) {
    mexErrMsgIdAndTxt(
      "SE2:complexInputs",
      "Don't know how to handle complex adjacency matrices.");
  }

  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_int_t membership;
  options opts = {};
  igraph_integer_t is_directed = -1;

  char *name;
  mwSize n_provided_optionals = nrhs - n_required_inputs;
  const mxArray **optionals = prhs + n_required_inputs;
  for (mwIndex i = 0; i < n_provided_optionals; i += 2) {
    if (!mxIsChar(optionals[i])) {
      mexErrMsgIdAndTxt("SE2:NameError",
                        "Expected argument %zu to be a string",
                        (mwIndex)n_required_inputs + i + 1);
    }
    name = mxArrayToString(optionals[i]);

    if (STREQ(name, "is_directed")) {
      is_directed = mxGetScalar(optionals[i + 1])
                    ? IGRAPH_DIRECTED : IGRAPH_UNDIRECTED;
    } else if (STREQ(name, "discard_transient")) {
      opts.discard_transient = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "independent_runs")) {
      opts.independent_runs = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "max_threads")) {
      opts.max_threads = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "minclust")) {
      opts.minclust = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "multicommunity")) {
      mexWarnMsgIdAndTxt("SE2:NotImplemented",
                         "Multicommunity detection has not been implemented, "
                         "returning a crisp partition.");
      opts.multicommunity = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "node_confidence")) {
      mexWarnMsgIdAndTxt("SE2:NotImplemented",
                         "Node confidence has not been implemnted, "
                         "ignoring request for node confidence.");
      opts.node_confidence = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "random_seed")) {
      opts.random_seed = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "subcluster")) {
      mexWarnMsgIdAndTxt("SE2:NotImplemented",
                         "Subclustering has not been implemented, "
                         "running main iteration only.");
      opts.subcluster = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "target_clusters")) {
      opts.target_clusters = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "target_partitions")) {
      opts.target_partitions = mxGetScalar(optionals[i + 1]);
    } else if (STREQ(name, "verbose")) {
      opts.verbose = mxGetScalar(optionals[i + 1]);
    } else {
      mexErrMsgIdAndTxt("SE2:NameError",
                        "Unknown name %s in name-value pair.",
                        name);
    }
  }

  if (is_directed < 0) {
    is_directed = mxIgraphIsDirected(prhs[0]);
  }

  mxIgraphGetGraph(prhs[0], &graph, &weights, is_directed);
  igraph_vector_int_init(&membership, 0);

  speak_easy_2(&graph, &weights, &opts, &membership);

  igraph_destroy(&graph);
  igraph_vector_destroy(&weights);

  plhs[0] = mxIgraphCreatePartition(&membership);
  igraph_vector_int_destroy(&membership);
}
