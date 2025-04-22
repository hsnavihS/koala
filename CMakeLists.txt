cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(koala VERSION 1.0)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(koala wasm_main.cpp main.cpp ${SOURCE_FILES})
target_include_directories(koala PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".js")

  # fexceptions should be set at both compile and link time to enable exception catching
  target_compile_options(koala PRIVATE -fexceptions)

  target_link_options(koala PRIVATE
  "-sEXPORTED_FUNCTIONS=['_interpret']"
  "-sEXPORTED_RUNTIME_METHODS=['ccall']"
  "-fexceptions"
)
endif()
