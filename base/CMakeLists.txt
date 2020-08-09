cmake_minimum_required(VERSION 3.16)
PROJECT(WebSever)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(CXX_FLAGS
    -g
    -Wall
    -std=c++11
    -D_PTHREADS
    -Wno-unused-parameter
)

SET(SRCS
    util.cpp
    channel.cpp
    epoll.cpp
    EventLoop.cpp
    Thread.cpp
    main.cpp
)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
include_directories(${PROJECT_SOURCE_DIR}/base)

add_executable(WebSever ${SRCS})
#target_link_libraries(WebSever libserver_base)

#add_subdirectory(base)