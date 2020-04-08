cmake_minimum_required(VERSION 3.10.2)
project(STLite_mapA)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g")
set(src_dir
	exceptions.hpp
	utility.hpp
	map.hpp
	code.cpp)
add_executable(STLite_mapA ${src_dir})