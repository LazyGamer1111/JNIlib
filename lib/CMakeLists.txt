cmake_minimum_required(VERSION 3.23)

project(JNIfull)

add_library(native SHARED hello.cpp)
include_directories(native ./include)
include_directories(native ../include)
target_link_libraries(native pio)
include_directories(native $ENV{JAVA_HOME}/include)
include_directories(native $ENV{JAVA_HOME}/include/linux)