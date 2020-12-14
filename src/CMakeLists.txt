cmake_minimum_required(VERSION 3.10)

# set the project name
project(shimmerlang VERSION ${BUILD_VERSION})

# add the executable
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC
     "*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(shimmerlang ${SRC})