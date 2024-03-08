import sys

sys.path = ["./python"] + sys.path
import igraph as ig

from speakeasy2 import cluster

g = ig.Graph.Famous("Zachary")
memb = cluster(g, target_partitions=10)
