import igraph as ig

from speakeasy2 import cluster

g = ig.Graph.Famous("Zachary")
memb = cluster(g, target_partitions=10, verbose=True)

g.es["weight"] = [1 for _ in range(g.ecount())]
memb = cluster(g, target_partitions=10, verbose=True)
