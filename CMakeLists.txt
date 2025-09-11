cmake_minimum_required(VERSION 3.28.3)
project(tcp_implementation)

set(CMAKE_CXX_STANDARD 17)

add_executable(tcp_implementation
        src/main.cpp
        src/ThreadManager.cpp
        include/ThreadManager.h
        src/Request.cpp
        include/Request.h
        include/HttpMethod.h
        src/Router.cpp
        include/Router.h
)
