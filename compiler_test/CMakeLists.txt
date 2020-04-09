find_package(GTest MODULE REQUIRED)
find_package(Threads REQUIRED)

add_custom_target(
        rsc
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/parse
)

add_custom_target(
        mvrsc
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/
        DEPENDS rsc
)

include_directories(${GTEST_INCLUDE_DIR})
aux_source_directory(tests DIR_TESTS)

file(COPY resources DESTINATION .)

aux_source_directory(../compiler/src DIR_SRC)
list(REMOVE_ITEM DIR_SRC ../compiler/src/main.cpp)

add_executable(compiler_test ${DIR_TESTS} ${DIR_SRC})
target_link_libraries(compiler_test ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(compiler_test rsc)
add_dependencies(compiler_test mvrsc)

enable_testing()
add_test(NAME compiler_test COMMAND $<TARGET_FILE:compiler_test>)
