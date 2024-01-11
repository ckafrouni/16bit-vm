cmake_minimum_required(VERSION 3.10)
project(vm) # TODO: change project name

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb -O3")

include_directories(SYSTEM RECURSE "include")

file(GLOB_RECURSE VM_SOURCES "src/vm/*.cpp")
file(GLOB_RECURSE COMPILER_SOURCES "src/assembler/*.cpp")

add_executable(vm ${VM_SOURCES}) # TODO: change executable name
add_executable(cc ${COMPILER_SOURCES}) # TODO: change executable name

set(CMAKE_COMMAND "${CMAKE_COMMAND}")
