cmake_minimum_required(VERSION 3.0.0)
project(TestProject VERSION 0.1.0)

set(GTEST_ROOT "C:/Program Files (x86)/googletest-distribution")  #Give a manual distribution for gtest folder
include_directories(${GTEST_ROOT}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Tests)

find_package(GTest)

add_executable(AsterixTest 
main_test.cpp
)

add_custom_target(
    COMMAND AsterixTest > test_output.txt # First Tests, Later write to txt
)

target_link_libraries(AsterixTest 
#${CMAKE_CURRENT_SOURCE_DIR}/../lib/libgtest_main.a
#${CMAKE_CURRENT_SOURCE_DIR}/../lib/libgtest.a  
${GTEST_LIBRARY} # This comment sections are an alternative way to add gtest to CMake
)

add_test(
    NAME AsterixTest
    COMMAND AsterixTest
)