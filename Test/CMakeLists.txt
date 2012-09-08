include_directories (../Scanner/)
include_directories (../MicroParser/)

add_executable (ScannerTest ScannerTest.cpp)
add_executable (MicroParserTest MicroParserTest.cpp)

# Link the executable 
target_link_libraries (ScannerTest Scanner)
target_link_libraries (MicroParserTest MicroParser Scanner)

INSTALL(TARGETS ScannerTest RUNTIME DESTINATION bin)
INSTALL(TARGETS MicroParserTest RUNTIME DESTINATION bin)

