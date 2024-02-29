# SpeakEasy 2 community detection

A port of the SpeakEasy 2 (SE2) community detection algorithm rewritten in C with the help of the [igraph C library](https://igraph.org/).
The original MATLAB code can be found at  [SE2](https://github.com/cogdishion/SE2).

Provides a C library and a MATLAB toolbox.

At the moment, implementation performs a subset of the matlab version. Specifically, this cannot perform subclustering, provide confidence for node membership, or order the nodes for plotting.

## Installation
This package uses CMake and git submodules for handling some of the dependencies. External dependencies are `bison`, `flex`, and `libxml2`.

To set download the git submodules use:

```bash
git submodule init && git submodule update --recursive
```

For CMake run:
```bash
cmake -B build .
cmake --build build
```
This will build mex files to `./build/src/mex` to run the toolbox in the current directory, you can link all mex files to `toolbox/private`. This will allow the mex files to be rebuilt without needing to repeatedly copy them to the toolbox.

## Use
There are two ways to run SE2, directly in C or in Matlab using the Matlab bindings.

### C
```C
#include "igraph.h"
#include "speak_easy_2.h"

int main()
{
  igraph_t graph;
  igraph_famous(&graph, "Zachary");
  igraph_vector_int_t membership;
  options opts = {};

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
```

The source can be compiled by using CMake and linking to the `SpeakEasy2` target.

Additionally, there are a number of options that can be added to the `opts` object to modify SE2. See [options](#Options).

### Matlab
```matlab
addpath("path/to/igraph-se2/toolbox")
adj = igraph.famous("Zachary"); % Requires matlab-igraph toolbox
membership = speakeasy2(adj);
```

Similarly to the C variant, several optional name-value pairs can be passed to modify how SE2 is run. See [options](#Options).


## Options

| Option | type | default | effect |
|--------|------|---------|--------|
| independent_runs | integer | 10 | number of independent runs to perform. Each run gets its own set of initial conditions. |
| target_partitions | integer | 5 | Number of partitions to find per independent run. |
| discard_transient | integer | 3 | Ignore this many partitions before tracking. |
| target_clusters | integer | dependent on size of graph | Expected number of clusters to find. Used for creating the initial conditions. The final partition is not constrained to having this many clusters. |
| random_seed | integer | randomly generated | a random seed for reproducibility. |
| max_threads | integer | Value returned by `omp_get_num_threads` | number of parallel threads to use. (Use 1 to prevent parallel processing.)|
| verbose | boolean | false | Whether to print extra information about the running process. |

In Matlab options are supplied as name-value pairs:

```matlab
speakeasy2(adj, 'seed', 1234, 'verbose', true, 'independentRuns', 5);
```

See `help speakeasy2` for more information.

For C, set the values of the options structure:

```C
	...
	options opts = {
		random_seed = 1234,
		verbose = true,
		independent_runs = 5
	};

	speakeasy2(&graph, &opts, &membership);
	...
```
