matlab_add_mex(NAME mexSE2
  MODULE
  SRC SpeakEasy2.c
  LINK_TO SpeakEasy2 mxIgraph
  R2018a
)

target_link_libraries(SpeakEasy2
  PRIVATE ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY}
  PUBLIC mxIgraph
)

target_include_directories(mexSE2
  PRIVATE ${Matlab_INCLUDE_DIRS}
)
