# Python SpeakEasy2 package

Provides the SpeakEasy2 community detection algorithm to cluster graph's stored as igraph's data type.

Example:

```python
 import igraph as ig
 import speakeasy2 as se2

 g = ig.Graph.Famous("Zachary")
 memb = se2.cluster(g)
```

## Installation

speakeasy2 is available from pypi so it can be installed with `pip` or other package managers. For building from source see the [github page](https://github.com/DavidRConnell/igraph-SE2).
