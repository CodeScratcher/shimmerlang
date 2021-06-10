cmake_minimum_required(VERSION 3.10)

# add the executable
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
file(GLOB SRC
     "*.cpp"
     "*.c"
)
set(VERSION ${BUILD_NUMBER})
configure_file(CMakeConfig.h.in CMakeConfig.h @ONLY)


include_directories(${PROJECT_SOURCE_DIR})
# add_subdirectory(linenoise-ng)
add_executable(shimmerlang ${SRC})

target_include_directories(shimmerlang PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
install(DIRECTORY lib/ DESTINATION /usr/lib/shimmer)
install(TARGETS shimmerlang RUNTIME DESTINATION /usr/local/bin)