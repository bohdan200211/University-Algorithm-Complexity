cmake_minimum_required(VERSION 3.17)
project(B__tree)

set(CMAKE_CXX_STANDARD 14)

add_executable(B__tree main.cpp B_Tree.cpp B_Tree.h B_Node.cpp B_Node.h)