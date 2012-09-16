include_directories (../include/)

add_executable (ScannerTest ScannerTest.cpp)
add_executable (MicroParserTest MicroParserTest.cpp)

# Link the executable 
target_link_libraries (ScannerTest Scanner Token)
target_link_libraries (MicroParserTest MicroParser Scanner Token)

INSTALL(TARGETS ScannerTest RUNTIME DESTINATION bin)
INSTALL(TARGETS MicroParserTest RUNTIME DESTINATION bin)

