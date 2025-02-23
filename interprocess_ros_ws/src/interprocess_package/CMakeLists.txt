cmake_minimum_required(VERSION 3.8)
project(interprocess_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Add your executable(s)
add_executable(interprocess_node src/main.cpp)  # Ensure the source path is correct

# Link against ROS 2 rclcpp and Boost using ament_target_dependencies
ament_target_dependencies(interprocess_node rclcpp Boost)

# Install the executable (optional)
install(
  TARGETS
    interprocess_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
