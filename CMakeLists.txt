cmake_minimum_required(VERSION 2.8)
project(CBDC)

## Project specific configurations go here

# enable c++17 support
set (CMAKE_CXX_FLAGS "-std=c++17 -Wall ${CMAKE_CXX_FLAGS}")

# create the main executable
## add additional .cpp files if needed
add_executable(main prototype.cpp classes.cpp invokeHamilton.cpp )
