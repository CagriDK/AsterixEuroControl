cmake_minimum_required(VERSION 3.0.0)
project(pAsterix VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

SET(ProjectMain_INC_Dirr ${CMAKE_CURRENT_SOURCE_DIR}/include/)
SET(ProjectMain_SRC_Dirr ${CMAKE_CURRENT_SOURCE_DIR}/src/)
FILE(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${ProjectMain_INC_Dirr}*.h")
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${ProjectMain_SRC_Dirr}*.cpp")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include asterix-messages
include_directories(CATMessages)
link_directories(CATMessages)
add_subdirectory(CATMessages)

#include GTest
add_subdirectory(GTest)

add_executable(pAsterix 
main.cpp
${HEADER_FILES}
${SOURCE_FILES}
)

set(COMMON_LIBS Asterix)

target_link_libraries(pAsterix ${COMMON_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
