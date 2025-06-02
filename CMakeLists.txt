cmake_minimum_required(VERSION 3.28)
project(BaoRT)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    main.cpp
    src/common/utils.cpp
    src/tests/print_test.cpp
    src/tests/main_test.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/extern/asmjit/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libasmjit.dylib
)
