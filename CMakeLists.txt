cmake_minimum_required(VERSION 3.22)

project(mqtt_ssl)

set(CMAKE_CXX_STANDARD 11)

add_definitions("-g -Os -Wall -Werror -fPIC")

file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/service/*.cpp")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/service
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SRC_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} mosquittopp)