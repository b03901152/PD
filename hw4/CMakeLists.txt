cmake_minimum_required(VERSION 2.8)

project(Router)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++11 -fopenmp -ffast-math")

add_executable(
	Router
	src/main.cpp
	src/FloorPlan.cpp
	src/SpanningTree.cpp
	src/SteinerTree.cpp
	src/SteinerTreeRect.cpp
	src/Graph/Graph.cpp
	src/GnuplotLivePlotter/GnuplotLivePlotter.cpp
)

find_package(Boost REQUIRED COMPONENTS system)

target_link_libraries(
	Router
	${Boost_SYSTEM_LIBRARY} 
)
