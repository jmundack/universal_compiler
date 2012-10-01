include_directories (../include/)

add_library (UCGrammerAnalyzer GrammerAnalyzer.h GrammerAnalyzer.cpp)
INSTALL(TARGETS UCGrammerAnalyzer ARCHIVE DESTINATION lib)
