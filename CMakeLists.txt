cmake_minimum_required(VERSION 4.0)
project(tcp_implementation)

set(CMAKE_CXX_STANDARD 17)

add_executable(tcp_implementation main.cpp
        ThreadManager.cpp
        ThreadManager.h
        Request.cpp
        Request.h
        HttpMethod.h
)
