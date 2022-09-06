cmake_minimum_required(VERSION 3.0)
PROJECT(FTP_SERVER)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_BUILD_TYPE "Debug")


add_executable(server main.cpp ${SRC})

target_link_libraries(server -lACE)