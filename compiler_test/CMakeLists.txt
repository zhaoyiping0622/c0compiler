cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)
aux_source_directory(tests DIR_TESTS)
aux_source_directory(../compiler/src DIR_SRC)
file(COPY resources DESTINATION .)
list(REMOVE_ITEM DIR_SRC ../compiler/src/main.cpp)
add_executable(compiler_test ${DIR_TESTS} ${DIR_SRC})
target_link_libraries(compiler_test gtest gtest_main -pthread)
