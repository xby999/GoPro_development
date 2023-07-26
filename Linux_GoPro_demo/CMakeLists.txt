cmake_minimum_required(VERSION 3.5.1)

project (GoProStreamDemo)
# Compile options
add_compile_options(-std=c++14)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "./out")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall")
set(FFMPEG_PATH "/usr/local/ffmpeg4.2")
set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_ROOT}/CMake)

# Find ffmpeg
find_package(FFMPEG REQUIRED)
#find_package(GStreamer REQUIRED)

# Header path
include_directories(
    "./inc"
    "/usr/local/ffmpeg/"
)

aux_source_directory(. SRC_LIST)
link_libraries(pthread ${FFMPEG_LIBRARIES} )
add_executable(GoPro ${SRC_LIST})
