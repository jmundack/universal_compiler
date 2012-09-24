include_directories (../include/)

add_executable (UCScannerTest ScannerTest.cpp)

# Link the executable 
target_link_libraries (UCScannerTest UCScanner Token)

INSTALL(TARGETS UCScannerTest RUNTIME DESTINATION bin)

