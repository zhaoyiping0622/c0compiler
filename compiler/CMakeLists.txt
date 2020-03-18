cmake_minimum_required(VERSION 3.10)
project(compiler)

aux_source_directory(src DIR_SRC)

add_executable(compiler ${DIR_SRC})