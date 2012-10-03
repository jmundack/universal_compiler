include_directories (../include/)

add_library (UCPredictGenerator PredictGenerator.h PredictGenerator.cpp)
INSTALL(TARGETS UCPredictGenerator ARCHIVE DESTINATION lib)
