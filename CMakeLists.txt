cmake_minimum_required(VERSION 3.0.0)
project(ynet VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(${PROJECT_NAME}
    src/buffer.cpp
    src/channel.cpp
    src/dispatcher.cpp
    src/event_loop_pool.cpp
    src/event_loop.cpp
    src/session.cpp
    src/tcp_server.cpp
    src/ynet/atomicops.h
    src/ynet/readerwriterqueue.h
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_auto_type
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src/
)

# target_link_directories(${PROJECT_NAME}
#     PRIVATE
# )

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        pthread
)