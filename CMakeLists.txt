cmake_minimum_required(VERSION 3.15)
project(webserver)

set(CMAKE_CXX_STANDARD 14)

add_executable(webserver main.cpp TCPListener.cpp TCPListener.h)