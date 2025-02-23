cmake_minimum_required(VERSION 3.10)
project(RepoB_SharedMemory)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost (ensure Interprocess is included)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Add executable
add_executable(repo_b src/main.cpp)

# Link Boost
target_include_directories(repo_b PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(repo_b PRIVATE ${Boost_LIBRARIES})