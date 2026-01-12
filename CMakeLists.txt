cmake_minimum_required(VERSION 3.10)
project(multithreaded-runtime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(runtime ${SOURCES})
target_include_directories(runtime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
