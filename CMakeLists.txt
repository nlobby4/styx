cmake_minimum_required(VERSION 3.22.1)
project(WebServer)
# Build cJSON as static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(
    cjson
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON
    GIT_TAG v1.7.18
)

FetchContent_MakeAvailable(cjson)

# Tests are not built by default
option(BUILD_TESTS "Build test files? (Criterion install is needed)" OFF)

# Recursively collect all .c files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
#add_compile_options(-Wall -Werror -pedantic)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${cjson_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} cjson)

enable_testing()
if(BUILD_TESTS)
    file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.c)
    list(APPEND TEST_SOURCES ${SOURCES})
    list(REMOVE_ITEM TEST_SOURCES ${PROJECT_SOURCE_DIR}/src/main.c)
    add_executable(Tests ${TEST_SOURCES})
    target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_include_directories(Tests PRIVATE ${cjson_SOURCE_DIR})
    target_compile_definitions(Tests PRIVATE TEST)
    target_link_libraries(Tests criterion cjson)
endif()