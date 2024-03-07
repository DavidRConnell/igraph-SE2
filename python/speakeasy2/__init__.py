"""The SpeakEasy2 community detection algorithm.

This module provides a community detection algorithm for clustering nodes in a
graph. The method accepts graphs created with the python-igraph package.

Methods:
-------
cluster : The SpeakEasy2 community detection algorithm.

Constants:
---------
__version__ : C library version.

Example:
-------
   >>> import igraph as ig
   >>> import speakeasy2 as se2
   >>> g = ig.Graph.Famous("Zachary")
   >>> memb = se2.cluster(g)

"""

__all__ = ["cluster", "__version__"]

import igraph as _ig

from speakeasy2._speakeasy2 import SE2_VERSION
from speakeasy2._speakeasy2 import cluster as _cluster

__version__ = SE2_VERSION


def cluster(g: _ig.Graph, **kwds) -> _ig.VertexClustering:
    """Cluster a graph using the SpeakEasy2 community detection algorithm.

    Parameters
    ----------
    g : igraph.Graph
        The graph to cluster.
    weights : list[float], None
        Optional list of weights. Must have length equal to the number of edges
        in the graph.
    discard_transient : int
        The number of partitions to discard before tracking. Default 3.
    independent_runs : int
        How many runs SpeakEasy2 should perform. Default 10.
    max_threads : int
        The maximum number of threads to use. By default OpenMP will determine
        the number to use based on either the value of the environment variable
        OMP_NUM_THREADS or the number of available virtual cores.
    seed : int
        A seed to use for the random number generator for reproducible results.
    target_clusters : int
        The expected number of clusters to find (default is dependent on the
        size of the graph). This only impacts how many initial labels are used.
        The final number of labels may be different.
    target_partitions : int
        Number of partitions to find per independent run (default 5).
    verbose : bool
        Whether to provide additional information about the clustering or not.

    Returns
    -------
    memb : igraph.VertexClustering
        The detected community structure.

    """
    memb = _cluster(g, **kwds)
    return _ig.VertexClustering(g, membership=memb)
