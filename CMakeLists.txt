project(compiler)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
include_directories(compiler/include)
include_directories(compiler/thirdparty)
add_subdirectory(compiler)
link_libraries(stdc++)

enable_testing()
add_subdirectory(compiler_test)
