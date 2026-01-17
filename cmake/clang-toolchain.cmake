set(CMAKE_C_COMPILER clang CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER clang++ CACHE STRING "" FORCE)
# Explicitly add libc++ include path so clang-tidy and clang find standard headers
set(CMAKE_CXX_FLAGS "-stdlib=libc++ -isystem /usr/lib/llvm-14/include/c++/v1" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ -lc++abi" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_LIBRARIES "-stdlib=libc++ -lc++abi" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
