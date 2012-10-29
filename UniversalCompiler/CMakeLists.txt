project (universal_compiler)

message("Building ${CMAKE_PROJECT_NAME}")

add_subdirectory (Scanner)
add_subdirectory (GrammerAnalyzer)
add_subdirectory (PredictGenerator)
add_subdirectory (TableGenerator)
add_subdirectory (ParserDriver)
add_subdirectory (MicroCompiler)
add_subdirectory (Test)
