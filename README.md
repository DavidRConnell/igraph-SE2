# SpeakEasy 2 community detection

A port of the SpeakEasy 2 (SE2) community detection algorithm rewritten in C with the help of the [igraph C library](https://igraph.org/).
The original Matlab code can be found at  [SE2](https://github.com/cogdishion/SE2).
This creates a C library that can be used to run SE2 and will provide matlab bindings.

At the moment, implementation performs a subset of the matlab version. Specifically, this cannot perform subclustering, provide confidence for node membership, or order the nodes for plotting.

## Installation
To install run the `make` command.

### For the C library

```bash
make lib
```

Installation of the C library requires igraph being installed in a known location.

### For Matlab toolbox

```bash
make toolbox
```

Note: The [matlab-igraph](https://github.com/DavidRConnell/matlab-igraph) toolbox is used in compiling the SE2 toolbox, this contains its own copy of igraph, so it does not need to installed separately. Consequently, *if the C library has been compiled already, it will be linked to the wrong copy of igraph so run `make clean-dist` before making the toolbox*.

Note: Since matlab-igraph does compile igraph, it requires igraph's dependencies to be available. See the [igraph installation page](https://igraph.org/c/html/latest/igraph-Installation.html). In short you need at least a C compiler and CMake.

The installation works only for linux OSes, as of now.
I believe it should be easy to modify installation for other OSes but have yet to test it.

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

Additionally, there are a number of options that can be added to the `opts` object to modify SE2. See [options](#Options).

### Matlab
```matlab
addpath("path/to/igraph-se2/toolbox")
adj = igraph.famous("Zachery"); % Requires matlab-igraph toolbox
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
speakeasy2(adj, 'random_seed', 1234, 'verbose', true, 'independent_runs', 5);
```

For C, use change the values of the options structure:

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
