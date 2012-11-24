include_directories (../include/)

set (MODULES Scanner GrammerAnalyzer PredictGenerator TableGenerator ParserDriver 
   ParserDriverWithSemanticStack MicroCompiler)
set (LIBRARIES Token)
foreach (module ${MODULES})
   list(APPEND TESTS ${module}Test)
   set(LIBRARIES UC${module} ${LIBRARIES})
endforeach(module)
message(Tests : ${TESTS})
message(Libraries : ${LIBRARIES})
#set (TESTS ScannerTest GrammerAnalyzerTest PredictGeneratorTest
#   TableGeneratorTest ParserDriverTest ParserDriverWithSemanticStackTest MicroCompilerTest)
#set (LIBRARIES UCMicroCompiler UCParserDriver UCTableGenerator UCPredictGenerator
#   UCGrammerAnalyzer UCScanner Token)

foreach (test ${TESTS})
   message(Test : ${test})
   add_executable(UC${test} ${test}.cpp)
   target_link_libraries(UC${test} ${LIBRARIES})
   install(TARGETS UC${test} RUNTIME DESTINATION bin)
endforeach(test)
