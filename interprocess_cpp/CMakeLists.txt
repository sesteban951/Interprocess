cmake_minimum_required(VERSION 3.10)
project(RepoB_SharedMemory)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost (ensure Interprocess is included)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Add executable for main (existing setup)
add_executable(main src/main.cpp)
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${Boost_LIBRARIES})

# Linux (POSIX) examples
add_executable(process1 tests/main1.cpp)
add_executable(process2 tests/main2.cpp)

# Link pthread for POSIX thread support
target_link_libraries(process1 PRIVATE pthread)
target_link_libraries(process2 PRIVATE pthread)
