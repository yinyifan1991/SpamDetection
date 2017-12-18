cmake_minimum_required(VERSION 3.8)
project(SpamDetection)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Decomposition.cpp Decomposition.h build_graph.cpp build_graph.h evaluation.cpp evaluation.h main.h reviewer.cpp reviewer.h)
add_executable(SpamDetection ${SOURCE_FILES})