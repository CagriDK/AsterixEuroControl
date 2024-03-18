cmake_minimum_required(VERSION 3.0.0)
project(Asterix VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# GLOB komutu ile 'lib' klasöründeki tüm .cpp ve .h dosyalarını bul
file(GLOB LIB_SOURCES
     "../lib/*.cpp"
     "../lib/*.h"
)

# GLOB komutu ile 'inc' klasöründeki tüm .cpp ve .h dosyalarını bul
file(GLOB INC_SOURCES
     "../include/*.h"
)

file(GLOB SRC_SOURCES
     "../src/*.cpp"
)

# GLOB komutu ile 'lib' klasöründeki tüm .cpp ve .h dosyalarını bul
file(GLOB CAT_MESSAGES
     "*.h"
     "include/*.h"
     "categories-types/*.h"
     "tools/include/*.h"
     "tools/include/parser/*.h"
     "tools/include/serializer/*.h"

     "*.cpp"
     "src/*.cpp"
     "tools/src/*.cpp"
     "tools/src/parser/*.cpp"
     "tools/src/serializer/*.cpp"
)

add_library(${PROJECT_NAME}
    ${LIB_SOURCES}
    ${INC_SOURCES}
    ${SRC_SOURCES} 
    ${CAT_MESSAGES} 
)