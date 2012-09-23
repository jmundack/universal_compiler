include_directories (../include/)

add_executable (ACScannerTest ScannerTest.cpp)
add_executable (ACMicroParserTest MicroParserTest.cpp)

# Link the executable 
target_link_libraries (ACScannerTest ACScanner Token)
target_link_libraries (ACMicroParserTest ACMicroParser ACScanner Token)

INSTALL(TARGETS ACScannerTest RUNTIME DESTINATION bin)
INSTALL(TARGETS ACMicroParserTest RUNTIME DESTINATION bin)

