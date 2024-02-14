cmake_minimum_required(VERSION 3.0.0)
project(pAsterix VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# GLOB komutu ile 'lib' klasöründeki tüm .cpp ve .h dosyalarını bul
file(GLOB LIB_SOURCES
     "lib/*.cpp"
     "lib/*.h"
)

# GLOB komutu ile 'inc' klasöründeki tüm .cpp ve .h dosyalarını bul
file(GLOB INC_SOURCES
     "inc/jasterix/*.cpp"
     "inc/jasterix/*.h"
)

file(GLOB SRC_SOURCES
     "CATMessages/items/*.cpp"
     "CATMessages/items/*.h"
     "CATMessages/categories-types/*.h"
)

# GLOB komutu ile 'lib' klasöründeki tüm .cpp ve .h dosyalarını bul
file(GLOB CAT_MESSAGES
     "CATMessages/*.cpp"
     "CATMessages/*.h"
     "CATMessages/include/*.h"
     "CATMessages/src/*.cpp"
)

add_executable(pAsterix 
main.cpp
${LIB_SOURCES}
${INC_SOURCES}
${SRC_SOURCES} 
${CAT_MESSAGES} 
)

include_directories(inc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
