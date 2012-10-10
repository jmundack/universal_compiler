include_directories (../include/)

add_executable (UCScannerTest ScannerTest.cpp)
add_executable (UCGrammerAnalyzerTest GrammerAnalyzerTest.cpp)
add_executable (UCPredictGeneratorTest PredictGeneratorTest.cpp)
add_executable (UCTableGeneratorTest TableGeneratorTest.cpp)

# Link the executable 
target_link_libraries (UCScannerTest UCScanner Token)
target_link_libraries (UCGrammerAnalyzerTest UCGrammerAnalyzer)
target_link_libraries (UCPredictGeneratorTest UCPredictGenerator UCGrammerAnalyzer)
target_link_libraries (UCTableGeneratorTest UCTableGenerator UCPredictGenerator UCGrammerAnalyzer)

INSTALL(TARGETS UCScannerTest RUNTIME DESTINATION bin)
INSTALL(TARGETS UCGrammerAnalyzerTest RUNTIME DESTINATION bin)
INSTALL(TARGETS UCPredictGeneratorTest RUNTIME DESTINATION bin)
INSTALL(TARGETS UCTableGeneratorTest RUNTIME DESTINATION bin)

