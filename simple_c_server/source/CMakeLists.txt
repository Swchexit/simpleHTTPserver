cmake_minimum_required(VERSION 3.10)
project(introCN_Proj C)

set(CMAKE_C_STANDARD 99)

add_executable(introCN_Proj main.c myhttpd.c)

add_dependencies(introCN_Proj copy_html_files)

add_custom_target(
        copy_html_files
        ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/*.html ${CMAKE_BINARY_DIR}
)
