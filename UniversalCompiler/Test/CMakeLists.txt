include_directories (../include/)

set (TESTS ScannerTest GrammerAnalyzerTest PredictGeneratorTest
   TableGeneratorTest ParserDriverTest)
set (LIBRARIES UCParserDriver UCTableGenerator UCPredictGenerator
   UCGrammerAnalyzer UCScanner Token)

foreach (test ${TESTS})
   add_executable(UC${test}Test ${test}.cpp)
   target_link_libraries(UC${test}Test ${LIBRARIES})
   install(TARGETS UC${test}Test RUNTIME DESTINATION bin)
endforeach(test)
