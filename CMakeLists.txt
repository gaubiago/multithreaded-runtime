cmake_minimum_required(VERSION 3.10)
project(multithreaded-runtime)

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(runtime ${SOURCES})
target_include_directories(runtime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
