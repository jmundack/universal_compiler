project (universal_compiler)

message("Building ${CMAKE_PROJECT_NAME}")

add_subdirectory (Scanner)
add_subdirectory (GrammerAnalyzer)
add_subdirectory (PredictGenerator)
add_subdirectory (Test)
