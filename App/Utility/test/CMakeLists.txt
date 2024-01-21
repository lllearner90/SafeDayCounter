cmake_minimum_required(VERSION 3.20)
project(TEST_Utility_conv)

# Find Google Test
find_package(GTest REQUIRED)

# Add the source files for the tests
add_executable(TEST_Utility_conv conv_test.cpp)

# Create a library from conv.cpp
add_library(ConvLibrary ../conv.cpp)

# Link the test executable against the library
target_link_libraries(TEST_Utility_conv PRIVATE GTest::GTest GTest::Main ConvLibrary)

# Register the tests
add_test(NAME TEST_Utility_conv COMMAND TEST_Utility_conv)