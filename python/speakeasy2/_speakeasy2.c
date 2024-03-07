#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <Python.h>
#include <speak_easy_2.h>
#include <se2_version.h>
#include <igraph.h>
#include <igraphmodule_api.h>

static void py_sequence_to_igraph_vector_i(PyObject* seq,
    igraph_vector_t* vec)
{
  size_t n_edges = PySequence_Size(seq);
  igraph_vector_init(vec, n_edges);
  for (size_t i = 0; i < n_edges; i++) {
    VECTOR(*vec)[i] = PyFloat_AsDouble(PySequence_GetItem(seq, i));
  }
}

static PyObject* igraph_vector_int_to_py_list_i(igraph_vector_int_t* vec)
{
  PyObject* res = PyList_New(igraph_vector_int_size(vec));
  for (igraph_integer_t i = 0; i < igraph_vector_int_size(vec); i++) {
    PyList_SetItem(res, i, PyLong_FromLong(VECTOR(*vec)[i]));
  }

  return res;
}

static PyObject* cluster(PyObject* Py_UNUSED(dummy), PyObject* args,
                         PyObject* kwds)
{
  PyObject* py_graph_obj = NULL;
  PyObject* py_weights_obj = NULL;
  igraph_t* graph;
  igraph_vector_t weights;
  char* kwlist[] = {
    "graph",
    "weights",
    "discard_transient",
    "independent_runs",
    "max_threads",
    "seed",
    "target_clusters",
    "target_partitions",
    "verbose",
    NULL
  };
  int discard_transient = 0;
  int independent_runs = 0;
  int max_threads = 0;
  int seed = 0;
  int target_clusters = 0;
  int target_partitions = 0;
  int verbose = false;
  igraph_vector_int_t memb;
  PyObject* py_memb_obj;

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|Obbbbbbp", kwlist,
                                   &py_graph_obj,
                                   &py_weights_obj,
                                   &discard_transient,
                                   &independent_runs,
                                   &max_threads,
                                   &seed,
                                   &target_clusters,
                                   &target_partitions,
                                   &verbose)) {
    return NULL;
  }

  se2_options opts = {
    .discard_transient = discard_transient,
    .independent_runs = independent_runs,
    .max_threads = max_threads,
    .random_seed = seed,
    .target_clusters = target_clusters,
    .target_partitions = target_partitions,
    .verbose = verbose,
  };

  graph = PyIGraph_ToCGraph(py_graph_obj);

  if (target_clusters > igraph_vcount(graph)) {
    PyErr_SetString(PyExc_ValueError,
                    "Number of target clusters cannot exceed the number of "
                    "nodes in the graph.");
  }

  if (py_weights_obj && PySequence_Check(py_weights_obj)) {
    py_sequence_to_igraph_vector_i(py_weights_obj, &weights);
    if (igraph_vector_size(&weights) != igraph_ecount(graph)) {
      PyErr_SetString(PyExc_ValueError,
                      "Number of weights does not match number of edges in graph.");
      return NULL;
    }
    speak_easy_2(graph, &weights, &opts, &memb);
  } else {
    igraph_vector_init(&weights, igraph_ecount(graph));
    for (igraph_integer_t i = 0; i < igraph_ecount(graph); i++) {
      VECTOR(weights)[i] = 1;
    }
    speak_easy_2(graph, &weights, &opts, &memb);
  }
  igraph_vector_destroy(&weights);

  py_memb_obj = igraph_vector_int_to_py_list_i(&memb);
  igraph_vector_int_destroy(&memb);

  return py_memb_obj;
}

static PyMethodDef SpeakEasy2Methods[] = {
  {"cluster", (PyCFunction)(void(*)(void))cluster, METH_VARARGS | METH_KEYWORDS, NULL},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef speakeasy2_module = {
  .m_base = PyModuleDef_HEAD_INIT,
  .m_name = "_speakeasy2",
  .m_size = -1,
  .m_methods = SpeakEasy2Methods
};

PyMODINIT_FUNC PyInit__speakeasy2(void)
{
  PyObject* m = PyModule_Create(&speakeasy2_module);
  if (m == NULL) {
    return NULL;
  }

  if (import_igraph() < 0) {
    return NULL;
  }

  PyModule_AddStringMacro(m, SE2_VERSION);

  return m;
}
