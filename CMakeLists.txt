cmake_minimum_required(VERSION 3.2.0)
project(WebRadar VERSION 0.0.1 LANGUAGES CXX)
set(THREAD_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)

add_subdirectory(src)