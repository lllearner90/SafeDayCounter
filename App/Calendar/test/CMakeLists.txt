cmake_minimum_required(VERSION 3.20)
project(TEST_Calendar)

# Find Google Test
find_package(GTest REQUIRED)

# Add the source files for the tests
add_executable(TEST_Calendar test_calendar.cpp)

# Create a library from calendar.cpp
add_library(CalendarLibrary ../calendar.cpp)

# Link the test executable against the library
target_link_libraries(TEST_Calendar PRIVATE GTest::GTest GTest::Main CalendarLibrary)

# Register the tests
add_test(NAME TEST_Calendar COMMAND TEST_Calendar)