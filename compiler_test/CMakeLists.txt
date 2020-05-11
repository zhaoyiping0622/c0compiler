find_package(GTest MODULE REQUIRED)
find_package(Threads REQUIRED)

add_custom_target(
        parseResources
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/parse
)

add_custom_target(
        generatorResources
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/generator
)

add_custom_target(
        mvrsc
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/
        DEPENDS parseResources generatorResources
)

include_directories(${GTEST_INCLUDE_DIR})
aux_source_directory(tests DIR_TESTS)

file(COPY resources DESTINATION .)

aux_source_directory(../compiler/src DIR_SRC)
list(REMOVE_ITEM DIR_SRC ../compiler/src/main.cpp)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage")

add_executable(compiler_test ${DIR_TESTS} ${DIR_SRC})
target_link_libraries(compiler_test ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(compiler_test gmock)
add_dependencies(compiler_test parseResources)
add_dependencies(compiler_test generatorResources)
add_dependencies(compiler_test mvrsc)

enable_testing()
add_test(NAME compiler_test COMMAND $<TARGET_FILE:compiler_test>)
