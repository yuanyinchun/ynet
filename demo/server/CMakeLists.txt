cmake_minimum_required(VERSION 3.0.0)
project(ynet-server VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}
    src/server.cpp
    src/main.cpp
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
        ${CMAKE_SOURCE_DIR}/../../release/ynet/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/../../release/ynet/lib
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ynet
)