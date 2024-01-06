cmake_minimum_required(VERSION 3.10)
project(vm) # TODO: change project name

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb -O2")

include_directories(SYSTEM RECURSE "include")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(vm ${SOURCES}) # TODO: change executable name
