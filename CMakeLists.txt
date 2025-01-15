cmake_minimum_required(VERSION 3.10)
project(skippy VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(
	skippy_lib STATIC
	src/glad.c
	src/shader.cpp
)

target_include_directories(skippy_lib PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_executable(skippy src/main.cpp)

if (MSVC)
	target_compile_options(skippy PRIVATE /W3)
else ()
	target_compile_options(skippy PRIVATE -Wall -Wextra -pedantic)
endif()

add_subdirectory(glfw)

target_link_libraries(skippy_lib PUBLIC glfw)

target_link_libraries(skippy PRIVATE skippy_lib)

