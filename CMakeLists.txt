cmake_minimum_required(VERSION 2.6)
project(slicplusplus)

set(CMAKE_CXX_COMPILER "/network-raid/opt/gcc/4.9.2-centos6/bin/g++492")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

include_directories($ENV{MAXCOMPILERDIR}/include/slic)
include_directories(include)

add_executable(PassthroughTest
	tests/PassthroughTest.cpp
)

add_executable(MappedElement
	tests/MappedElementTest.cpp
)
