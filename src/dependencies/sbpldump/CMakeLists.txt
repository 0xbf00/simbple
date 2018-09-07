cmake_minimum_required(VERSION 3.9)
project(sbpldump)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR})
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (LIBRARY_SRC_FILES
        ${PROJECT_SOURCE_DIR}/convert.cpp
        ${PROJECT_SOURCE_DIR}/convert.h)

set (EXECUTABLE_SRC_FILES
        ${PROJECT_SOURCE_DIR}/cli.cpp)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "-O3")

add_library(${PROJECT_NAME} STATIC ${LIBRARY_SRC_FILES})

# We cannot have two targets with the same name ordinarily -- even though
# the library is not called ${PROJECT_NAME} in the end. This workaround
# was mentioned here: https://cmake.org/pipermail/cmake/2006-October/011402.html
set(EXECUTABLE_PLACEHOLDER_NAME ${PROJECT_NAME}-bin)
add_executable(${EXECUTABLE_PLACEHOLDER_NAME} ${EXECUTABLE_SRC_FILES})
set_target_properties(${EXECUTABLE_PLACEHOLDER_NAME}
        PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(${EXECUTABLE_PLACEHOLDER_NAME} ${PROJECT_NAME})