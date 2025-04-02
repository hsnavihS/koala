cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(koala VERSION 1.0)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(koala main.cpp ${SOURCE_FILES})
target_include_directories(koala PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
