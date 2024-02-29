matlab_add_mex(NAME mexSE2
  MODULE
  SRC SpeakEasy2.c
  LINK_TO SpeakEasy2 mxIgraph
  R2018a
)

target_include_directories(mexSE2
  PRIVATE ${Matlab_INCLUDE_DIRS}
)

matlab_add_mex(NAME isdirected
  MODULE
  SRC isdirected.c
  LINK_TO mxIgraph
  R2018a
)

target_include_directories(isdirected
  PRIVATE ${Matlab_INCLUDE_DIRS}
)
