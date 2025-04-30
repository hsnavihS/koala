cmake_minimum_required(VERSION 3.10)
project(koala-lsp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(koalalsp main.cpp ${SOURCE_FILES})

target_include_directories(koalalsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
