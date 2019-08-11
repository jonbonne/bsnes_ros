cmake_minimum_required(VERSION 3.10)
project(sourcery)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -fPIC -O3 -DBUILD_PERFORMANCE -fwhole-program -flto -fno-fat-lto-objects -fno-strict-aliasing -fwrapv)
endif()


# set dependency requirements
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)

# Includes
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    "/usr/include"
)

add_executable(${PROJECT_NAME} "src/sourcery.cpp")

# Install C++ headers
install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp")

# Install project executables
install(TARGETS
  ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

# Export package dependencies
ament_export_dependencies(ament_cmake)
ament_export_libraries(${PROJECT_NAME})

ament_package()
