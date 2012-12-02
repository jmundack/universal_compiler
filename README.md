Universal-Compiler
==================

CSCI 5640 - Universal Compiler

How to build:
 --> cd build
 --> ./bootstrap.sh

TestData:
 --> Test input files can be found in the TestData directory

Assignment 1
To run Test (file name can be any file in the Test data directory)
 --> ./build/debug/bin/ACScannerTest <filename>

Assignment 2
To run Test (file name can be any file in the Test data directory)
 --> ./build/debug/bin/ACMicroParserTest <filename>

Assignment 3
To run Test (file name can be any file in the Test data directory) -- will print out the generated code
 --> ./build/debug/bin/ACMicroParserTest <filename>

Assignment 4
To run Test (file name can be any file in the Test data directory) -- will print out the generated code
 --> ./build/debug/bin/ACMicroParserTest <filename>
 --> ./build/debug/bin/UCMicroParserTest <filename>
both should out the same last line

Assignment 5
To run Test (file name can be any file in the Test data directory) -- will print out the results
 --> ./build/debug/bin/UCGrammerAnalyzerTest <filename>
Asuumption : adjoining terminal token are space separated

Assignment 6
To run Test (file name can be any file in the Test data directory) -- will print out the results
 --> ./build/debug/bin/UCPredictGeneratorTest <filename>

Assignment 7
To run Test (file name can be any file in the Test data directory) -- will print out the results
 --> ./build/debug/bin/UCTableGeneratorTest <filename>

Assignment 8
To run Test (file name can be any file in the Test data directory) -- will print out the results
 --> ./build/debug/bin/UCParserDriverTest <MicroGrammer.dat> <program to parse>

Assignment 9
To run Test (file name can be any file in the Test data directory) -- will print out the results
 --> ./build/debug/bin/UCMicroCompilerTest <MicroGrammer_w_action_symbols.dat> <program to parse>
NOTE: ACMicroParserTest and UCMicroCompilerTest should generate the same code for all programs (tested with test programs 1,2,4,5 in test data directory - 3 is an invalid program)

Assignment 11
To run Test (file name can be any file in the Test data directory) -- will print out the results (compiled code or error if there were errors)
 --> ./build/debug/bin/ACMicroParserTest <program to parse>

Updated source code can be retrieved from
git://github.com/jmundack/universal_compiler.git

Credits
-- Algorithms used in these implementations have been obtained from
   stilman-strategies website (written by Prof Boris Stilman for Universal Compiler class) 
