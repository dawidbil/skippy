cmake_minimum_required(VERSION 3.10)
project(skippy VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(skippy main.cpp glad.c)
target_include_directories(skippy PRIVATE
	"${PROJECT_SOURCE_DIR}/include"
)

add_subdirectory(glfw)
target_link_libraries(skippy glfw)

