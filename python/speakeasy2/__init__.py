__all__ = ["cluster"]

import igraph as ig

from speakeasy2._speakeasy2 import cluster as _cluster


def cluster(g: ig.Graph, **kwds) -> ig.VertexClustering:
    memb = _cluster(g, **kwds)
    return ig.VertexClustering(g, membership=memb)
