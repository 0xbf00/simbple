cmake_minimum_required(VERSION 3.9)
project(simbple)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR})

add_subdirectory("scheme_interpreter")

include_directories("${PROJECT_INCLUDE_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS}")

set(CMAKE_CXX_LINK_FLAGS "-framework CoreFoundation -framework Foundation -largp -lsandbox")

add_executable(
        simbple
        interpreter.c
        misc/argument_handling.c
        misc/argument_handling.h
        misc/misc.h
        sb/modifiers.c
        sb/modifiers.h
        sb/operations/interface.c
        sb/operations/operations.h
        sb/operations/types.h
        sb/operations/data.c
        sb/operations/data.h
        sb/operations/mem.c
        sb/operations/mem.h
        misc/scheme_support.h
        misc/scheme_support.c
        sb/sb.h
        sb/filters/filters.h
        sb/filters/data.c
        sb/filters/data.h
        sb/filters/types.h
        sb/filters/interface.c
        sb/filters/mem.c
        sb/filters/mem.h
        sb/meta-filters/meta-filters.h
        sb/meta-filters/types.h
        sb/meta-filters/definition.c
        sb/meta-filters/definition.h
        sb/meta-filters/interface.c
        sb/meta-filters/mem.c
        sb/meta-filters/mem.h
        sb/actions.c
        sb/actions.h
        sb/rule_base.cpp
        sb/rule_base.h
        sb/error.c
        sb/error.h
        sb/blob_handling.c
        sb/blob_handling.h
        misc/regex.c
        misc/regex.h
        misc/string.c
        misc/string.h
        misc/filesystem.c
        misc/filesystem.h
        misc/os_support.h
        misc/os_support.m
        sb/dyn_params.m
        sb/dyn_params.h 
        sb/evaluate.c 
        sb/evaluate.h 
        sb/verify.c 
        sb/verify.h
        platform_data/sierra/operations.c
        platform_data/sierra/operations.h
        platform_data/sierra/filters.c
        platform_data/sierra/filters.h
        platform_data/high_sierra/operations.c
        platform_data/high_sierra/operations.h
        platform_data/high_sierra/filters.c
        platform_data/high_sierra/filters.h
        platform_data/platforms.h
        platform_data/platforms.c
        )

target_link_libraries(simbple tinyscheme)
