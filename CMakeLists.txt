cmake_minimum_required(VERSION 3.28)
project(MilaCompilerFE VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_executable(mila ${SOURCES} ${HEADERS})

target_compile_options(mila PRIVATE -Wall -Wextra -pedantic)

target_include_directories(mila PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/declarations
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/expressions
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/statements
        ${CMAKE_CURRENT_SOURCE_DIR}/include/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/core)

target_include_directories(mila SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_options(mila PRIVATE ${LLVM_DEFINITIONS_LIST})

target_link_libraries(mila PRIVATE stdc++)

llvm_config(mila USE_SHARED support core irreader)

include(CTest)
if (BUILD_TESTING)
    file(GLOB_RECURSE MILA_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/samples/*.mila")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")

    # compile tests
    foreach (src ${MILA_SOURCES})
        get_filename_component(basename ${src} NAME_WE)
        add_test(NAME "compiler:${basename}" COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/mila" "${src}" "-o" "${CMAKE_CURRENT_BINARY_DIR}/tests/${basename}")
        set_tests_properties("compiler:${basename}" PROPERTIES
                FIXTURES_SETUP "${basename}"
                ENVIRONMENT "MILA_BIN=$<TARGET_FILE:mila>")
    endforeach ()

    # run tests
    file(GLOB MILA_OUTPUTS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/run/*.run[0-9]*.out")
    foreach (out ${MILA_OUTPUTS})
        get_filename_component(outname ${out} NAME)
        get_filename_component(extensionOut ${out} EXT)
        get_filename_component(basename ${out} NAME_WE)
        string(REPLACE "out" "in" extensionIn "${extensionOut}")
        set(inname "${basename}${extensionIn}")

        set(executable ${CMAKE_CURRENT_BINARY_DIR}/tests/${basename})
        set(outfile ${CMAKE_CURRENT_SOURCE_DIR}/tests/run/${outname})
        set(infile ${CMAKE_CURRENT_SOURCE_DIR}/tests/run/${inname})

        if (EXISTS "${infile}")
            add_test(NAME "run:${outname}" COMMAND
                    ${CMAKE_COMMAND}
                    -D executable=${executable}
                    -D expected=${outfile}
                    -D input=${infile}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.cmake)
        else ()
            add_test(NAME "run:${outname}" COMMAND
                    ${CMAKE_COMMAND}
                    -D executable=${executable}
                    -D expected=${outfile}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.cmake)
        endif ()
        set_tests_properties("run:${outname}" PROPERTIES FIXTURES_REQUIRED "${basename}")
    endforeach ()
endif ()
