__all__ = ["cluster", "__version__"]

import igraph as _ig

from speakeasy2._speakeasy2 import SE2_VERSION
from speakeasy2._speakeasy2 import cluster as _cluster

__version__ = SE2_VERSION


def cluster(g: _ig.Graph, **kwds) -> _ig.VertexClustering:
    memb = _cluster(g, **kwds)
    return _ig.VertexClustering(g, membership=memb)
