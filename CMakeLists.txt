cmake_minimum_required(VERSION 3.10)
project(bsnes_ros)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -fPIC -03 -DBUILD_PERFORMANCE -fwhole-program -flto -fno-fat-lto-objects -fno-strict-aliasing -fwrapv)
endif()

# Pybind11 Settings
set(PYTHON_VER "3.6")
set(PYBIND11_PYTHON_VERSION ${PYTHON_VER})
set(Python_ADDITIONAL_VERSIONS ${PYTHON_VER})
set(PYBIND11_CPP_STANDARD -std=c++14)

# set dependency requirements
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pybind11 REQUIRED)
find_package(PythonInterp "${PYTHON_VER}" REQUIRED)
find_package(PythonLibs "${PYTHON_VER}")
set(PYTHON_INCLUDE_DIRS "/usr/include/python${PYTHON_VER}")

set(REQLIBS
#    ${rclcpp_LIBRARIES}
    ${pybind11_LIBRARIES}
    python3.6m
    uuid
)
#set(PYBIND11_LTO_LINKER_FLAGS "${PYBIND11_LTO_LINKER_FLAGS} ${REQLIBS}")
# install the python package
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)
ament_python_install_package(${PROJECT_NAME})
ament_export_dependencies(ament_cmake_python)
ament_export_dependencies(rclpy)

# Includes, namely python
include_directories(
    ${PROJECT_SOURCE_DIR}/include
#    ${rclcpp_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    "/usr/include/SDL2"
    "/usr/include/gtksourceview-2.0"
    "/usr/include/libxml2"
    "/usr/include/gtk-2.0"
    "/usr/include/gio-unix-2.0/"
    "/usr/include/cairo"
    "/usr/include/pango-1.0"
    "/usr/include/atk-1.0"
    "/usr/include/pixman-1"
    "/usr/include/gdk-pixbuf-2.0"
    "/usr/include/libpng16"
    "/usr/include/harfbuzz"
    "/usr/include/glib-2.0"
    "/usr/lib/x86_64-linux-gnu/gtk-2.0/include"
    "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
    "/usr/include/freetype2"
    "/usr/include/libpng16"
    "/usr/include/freetype2"
)
link_directories(
    ${rclcpp_LIBRARY_DIRS}
    "/usr/lib/python${PYTHON_VER}/config-${PYTHON_VER}m-x86_64-linux-gnu/"
    "/usr/lib/x86_64-linux-gnu"
    "/usr/local/lib")

file(GLOB_RECURSE lzma_h_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/lzma/*.h")
file(GLOB_RECURSE lzma_c_files "${PROJECT_SOURCE_DIR}/src/lzma/*.c")
file(GLOB_RECURSE libco_h_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/libco/*.h")
file(GLOB_RECURSE libco_c_files "${PROJECT_SOURCE_DIR}/src/libco/*.c")
file(GLOB_RECURSE gb_h_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/gb/*.h")
file(GLOB_RECURSE gb_c_files "${PROJECT_SOURCE_DIR}/src/gb/*.c")

file(GLOB_RECURSE ruby_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/ruby/*.hpp")
file(GLOB_RECURSE ruby_cpp_files "${PROJECT_SOURCE_DIR}/src/ruby/*.cpp")
file(GLOB_RECURSE hiro_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/hiro/*.hpp")
file(GLOB_RECURSE hiro_cpp_files "${PROJECT_SOURCE_DIR}/src/hiro/*.cpp")
file(GLOB_RECURSE nall_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/nall/*.hpp")
file(GLOB_RECURSE nall_cpp_files "${PROJECT_SOURCE_DIR}/src/nall/*.cpp")
file(GLOB_RECURSE sfc_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/sfc/*.hpp")
file(GLOB_RECURSE sfc_cpp_files "${PROJECT_SOURCE_DIR}/src/sfc/*.cpp")
file(GLOB_RECURSE emulator_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/emulator/*.hpp")
file(GLOB_RECURSE emulator_cpp_files "${PROJECT_SOURCE_DIR}/src/emulator/*.cpp")
file(GLOB_RECURSE filter_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/filter/*.hpp")
file(GLOB_RECURSE filter_cpp_files "${PROJECT_SOURCE_DIR}/src/filter/*.cpp")
file(GLOB_RECURSE processor_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/processor/*.hpp")
file(GLOB_RECURSE processor_cpp_files "${PROJECT_SOURCE_DIR}/src/processor/*.cpp")

# build up libraries
add_library(lzma STATIC ${lzma_h_files} "include/${PROJECT_NAME}/lzma/lzma.hpp" ${lzma_c_files} "src/lzma/lzma.cpp")
add_library(libco STATIC ${libco_h_files} ${libco_c_files})
add_library(gb STATIC ${gb_h_files} ${gb_c_files})
add_definitions(-DGB_INTERNAL -DDISABLE_DEBUGGER -D_GNU_SOURCE -Wno-multichar)
add_library(sfc STATIC ${sfc_hpp_files} ${sfc_cpp_files})
add_library(emulator STATIC ${emulator_hpp_files} ${emulator_cpp_files})
add_library(filter STATIC ${filter_hpp_files} ${filter_cpp_files})
add_library(processor STATIC ${processor_hpp_files} ${processor_cpp_files})

add_library(ruby STATIC ${ruby_hpp_files} ${ruby_cpp_files})
add_definitions(-D_REENTRANT)
target_link_libraries(ruby GL Xv asound openal pulse pulse-simple ao udev)

add_library(hiro-gtk2 STATIC ${hiro_hpp_files} ${hiro_cpp_files})
add_definitions(-DHIRO_GTK=2)
target_link_libraries(hiro-gtk2 pthread cairo fontconfig freetype X11)
#gtksourceview-2.0 gtk-x11-2.0 gdk-x11-2.0 pangocairo-1.0 atk-1.0 gdk_pixbuf-2.0 gio-2.0 pangoft2-1.0 pango-1.0 gobject-2.0 glib-2.0)

add_library(nall STATIC ${nall_hpp_files} ${nall_cpp_files})
add_definitions(-DHIRO_GTK=2)
target_link_libraries(nall pthread dl rt)

#file(GLOB_RECURSE h_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
#file(GLOB_RECURSE c_files "${PROJECT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/target-bsnes/*.hpp")
file(GLOB_RECURSE cpp_files "${PROJECT_SOURCE_DIR}/src/target-bsnes*.cpp")

# Add Libraries/Executables
add_executable(${PROJECT_NAME} ${hpp_files} ${cpp_files})
target_link_libraries(${PROJECT_NAME} lzma libco gb sfc emulator filter processor ruby hiro-gtk2 nall)

#
# Add pybind11 modules
#
#pybind11_add_module(state_manager MODULE "src/state_manager.cpp")
#ament_target_dependencies(state_manager ${PROJECT_NAME} ${REQLIBS})
#target_link_libraries(state_manager PRIVATE pybind11::module ${PROJECT_NAME} ${REQLIBS})

## Install pybind11 targets
#install(TARGETS
#    state_manager
#  DESTINATION lib/python${PYTHON_VER}/site-packages/${PROJECT_NAME}
#)
#
# Done adding pybind11 modules
#

# Install C++ headers
install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp")

install(
  DIRECTORY Database Locale applications icons
  DESTINATION share)

# Install python scripts
#install(PROGRAMS scripts/test_pybind DESTINATION bin)

# Install project artifacts
install(TARGETS
  ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
#  DESTINATION lib/python${PYTHON_VER}/site-packages/${PROJECT_NAME}
)

# Export package dependencies
ament_export_dependencies(ament_cmake)
ament_export_dependencies(ament_cmake_python)
ament_export_dependencies(rclcpp)
ament_export_dependencies(rclpy)
ament_export_include_directories(include ${rclcpp_INCLUDE_DIRS})
ament_export_libraries(${PROJECT_NAME} ${rclcpp_LIBRARIES})

ament_package()
