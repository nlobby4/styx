cmake_minimum_required(VERSION 3.22.1)

project(WebServer)

# Recursively collect all .c files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

# Include the headers from the include directory
target_include_directories(WebServer PRIVATE ${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(WebServer cjson)