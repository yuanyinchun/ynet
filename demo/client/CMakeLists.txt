cmake_minimum_required(VERSION 3.0.0)
project(ynet-client VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}
    main.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_auto_type
)