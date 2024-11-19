cmake_minimum_required(VERSION 3.22.1)

project(WebServer)

set(SOURCES
    src/main.c
    src/utils/error.c
    src/utils/handleargs.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(WebServer PRIVATE ${PROJECT_SOURCE_DIR}/include)