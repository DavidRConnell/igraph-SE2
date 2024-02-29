file(GLOB src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(SpeakEasy2
  OBJECT
  ${src_files}
)

target_include_directories(SpeakEasy2
 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
