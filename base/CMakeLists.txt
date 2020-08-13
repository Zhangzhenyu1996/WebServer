SET(SRCS
    util.cpp
    channel.cpp
    epoll.cpp
    EventLoop.cpp
    Thread.cpp
    CountDownLatch.cpp
    main.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/base)

add_executable(WebSever ${SRCS})
add_library(base ${SRCS})
target_link_libraries(WebSever pthread)

add_subdirectory(tests)


