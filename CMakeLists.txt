cmake_minimum_required(VERSION 3.22.1)
project(WebServer)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(
    cjson
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON
    GIT_TAG acc76239bee01d8e9c858ae2cab296704e52d916
)

FetchContent_MakeAvailable(cjson)
# Recursively collect all .c files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Werror -pedantic)

add_executable(${PROJECT_NAME} ${SOURCES})


# Include the headers from the include directory
target_include_directories(WebServer PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(WebServer PRIVATE ${cjson_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(WebServer cjson)