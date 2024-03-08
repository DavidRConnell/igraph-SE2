# SpeakEasy 2 community detection

A port of the SpeakEasy 2 (SE2) community detection algorithm rewritten in C with the help of the [igraph C library](https://igraph.org/).
The original MATLAB code can be found at  [SE2](https://github.com/cogdishion/SE2).

Provides a C library and bindings for high level languages: python, MATLAB, and R.

At the moment, implementation performs a subset of the matlab version. Specifically, this cannot perform subclustering, provide confidence for node membership, or order the nodes for plotting.

## Building from source
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

### Python

The Python package can be built with `CMake` using the above command or can be built with `poetry`. Using `poetry` has the advantage of providing dev tools and the [python-igraph](https://github.com/igraph/python-igraph) package. To install the dependencies, cd into the python directory and run `poetry install --with=dev`. Then to build use `poetry build` (after building the main package with the above `CMake` commands).

## Installation

### MATLAB

Use the MATLAB add-on manager inside the MATLAB IDE. The [matlab-igraph](https://github.com/DavidRConnell/matlab-igraph) may also be useful (can also be found in the add-on manager).

### Python

The python package can be found on pypi. Any package manager that can install pypi packages, such as `pip` or `poetry`, can be used to install.

## Use

The same basic approach can be used for running SpeakEasy2 in any language. Obtain a graph that can be represented as in igraph's data type (such as a symmetric matrix in MATLAB or an igraph graph in python) then call the SpeakEasy2 function on the graph, providing any of the options from [options](#Options) to modify the behavior as desired. The default options should work well in must cases (though setting a random seed may be useful). The options vary slightly depending on the language's naming convention, see the languages help for more information on the language specific option names.

### C
```C
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
```

The source can be compiled by using CMake and linking to the `SpeakEasy2` target.

### Matlab
```matlab
adj = igraph.famous("Zachary"); % Requires matlab-igraph toolbox
membership = speakeasy2(adj);
```

### Python

``` python
import igraph as ig
import speakeasy2 as se2

g = igraph.Graph.Famous("Zachary")
membership = se2.cluster(g)
```

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

For C, set the values of the options structure:

```C
	...
	se2_options opts = {
		random_seed = 1234,
		verbose = true,
		independent_runs = 5
	};

	speak_easy_2(&graph, NULL, &opts, &membership);
	...
```

In MATLAB options are supplied as name-value pairs:

```matlab
speakeasy2(adj, 'seed', 1234, 'verbose', true, 'independentRuns', 5);
```

For MATLAB 2019 or later, option can also be supplied in a `name=value` format:

``` matlab
speakeasy2(adj, discardTransient=5, verbose=true);
```

See `help speakeasy2` for more information.
