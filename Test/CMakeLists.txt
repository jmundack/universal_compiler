include_directories (../Scanner/)

add_executable (ScannerTest ScannerTest.cpp)

# Link the executable to the Hello library.
target_link_libraries (ScannerTest Scanner)

INSTALL(TARGETS ScannerTest RUNTIME DESTINATION bin)

