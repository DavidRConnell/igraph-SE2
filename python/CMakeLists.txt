if(NOT Python3_FOUND)
  message(WARNING "Python not found skipping python library.")
endif()

if(Python3_VERSION_MINOR LESS 10)
  message(WARNING
    "Python version must be at least 3.10 found \
${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR} skipping python library.")
endif()

Python3_add_library(_speakeasy2 MODULE
  ${CMAKE_CURRENT_SOURCE_DIR}/speakeasy2/_speakeasy2.c)

target_link_libraries(_speakeasy2
  PUBLIC SpeakEasy2
)

target_include_directories(_speakeasy2
  PRIVATE
  ${PROJECT_SOURCE_DIR}/vendor/python-igraph/src/_igraph
  ${PROJECT_BINARY_DIR}/include
)
