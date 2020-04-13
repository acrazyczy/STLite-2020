cmake_minimum_required(VERSION 3.10.2)
project(STLite_deque)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -Wall -Wshadow -Wextra")
set(src_dir
	exceptions.hpp
	utility.hpp
	deque.hpp
	code.cpp)
add_executable(STLite_deque ${src_dir})