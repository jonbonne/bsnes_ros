cmake_minimum_required(VERSION 3.10)
project(bsnes_ros)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -fPIC -O3 -DBUILD_PERFORMANCE -fwhole-program -flto -fno-fat-lto-objects -fno-strict-aliasing -fwrapv)
endif()

# Pybind11 Settings
set(PYTHON_VER "3.6")
set(PYBIND11_PYTHON_VERSION ${PYTHON_VER})
set(Python_ADDITIONAL_VERSIONS ${PYTHON_VER})
set(PYBIND11_CPP_STANDARD -std=c++17)

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
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/lzma
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/libco
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/nall
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/gb/Core
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/hiro
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/hiro/core
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/hiro/extension
    ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/hiro/resource
    ${PROJECT_SOURCE_DIR}/src/hiro
    ${PROJECT_SOURCE_DIR}/src/hiro/core
    ${PROJECT_SOURCE_DIR}/src/hiro/extension
    ${PROJECT_SOURCE_DIR}/src/hiro/resource
#    ${rclcpp_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    "/usr/include"
    "/usr/local/include"
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

file(GLOB_RECURSE gb_h_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/gb/*.h")

file(GLOB_RECURSE ruby_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/ruby/*.hpp")
file(GLOB_RECURSE hiro_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/hiro/*.hpp")
file(GLOB_RECURSE sfc_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/sfc/*.hpp")
file(GLOB_RECURSE emulator_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/emulator/*.hpp")
file(GLOB_RECURSE filter_hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/filter/*.hpp")

add_definitions(-DGB_INTERNAL -DDISABLE_DEBUGGER -D_GNU_SOURCE -Wno-multichar)
add_definitions(-D_REENTRANT)
add_definitions(-DHIRO_GTK=2)

# build up libraries
add_library(lzma STATIC "src/lzma/lzma.cpp")

add_library(gb-apu STATIC "src/gb/Core/apu.c")
add_library(gb-camera STATIC "src/gb/Core/camera.c")
add_library(gb-debugger STATIC "src/gb/Core/debugger.c")
add_library(gb-display STATIC "src/gb/Core/display.c")
add_library(gb-gb STATIC "src/gb/Core/gb.c")
add_library(gb-joypad STATIC "src/gb/Core/joypad.c")
add_library(gb-mbc STATIC "src/gb/Core/mbc.c")
add_library(gb-memory STATIC "src/gb/Core/memory.c")
add_library(gb-printer STATIC "src/gb/Core/printer.c")
add_library(gb-random STATIC "src/gb/Core/random.c")
add_library(gb-rewind STATIC "src/gb/Core/rewind.c")
add_library(gb-save_state STATIC "src/gb/Core/save_state.c")
add_library(gb-sgb STATIC "src/gb/Core/sgb.c")
add_library(gb-sm83_cpu STATIC "src/gb/Core/sm83_cpu.c")
add_library(gb-sm83_disassembler STATIC "src/gb/Core/sm83_disassembler.c")
add_library(gb-symbol_hash STATIC "src/gb/Core/symbol_hash.c")
add_library(gb-timing STATIC "src/gb/Core/timing.c")

add_library(sfc-interface STATIC "src/sfc/interface/interface.cpp")
add_library(sfc-system STATIC "src/sfc/system/system.cpp")
add_library(sfc-controller STATIC "src/sfc/controller/controller.cpp")
add_library(sfc-cartridge STATIC "src/sfc/cartridge/cartridge.cpp")
add_library(sfc-memory STATIC "src/sfc/memory/memory.cpp")
add_library(sfc-cpu STATIC "src/sfc/cpu/cpu.cpp")
add_library(sfc-smp STATIC "src/sfc/smp/smp.cpp")
add_library(sfc-dsp STATIC "src/sfc/dsp/dsp.cpp")
add_library(sfc-ppu STATIC "src/sfc/ppu/ppu.cpp")
add_library(sfc-ppu-fast STATIC "src/sfc/ppu-fast/ppu.cpp")
add_library(sfc-expansion STATIC "src/sfc/expansion/expansion.cpp")
add_library(sfc-coprocessor STATIC "src/sfc/coprocessor/coprocessor.cpp")
add_library(sfc-slot STATIC "src/sfc/slot/slot.cpp")

add_library(emulator STATIC ${emulator_hpp_files} "src/emulator/emulator.cpp")
add_library(filter STATIC ${filter_hpp_files} "src/filter/filter.cpp")

add_library(processor-arm7tdmi STATIC "src/processor/arm7tdmi/arm7tdmi.cpp")
add_library(processor-gsu STATIC "src/processor/gsu/gsu.cpp")
add_library(processor-hg51b STATIC "src/processor/hg51b/hg51b.cpp")
add_library(processor-sm83 STATIC "src/processor/sm83/sm83.cpp")
add_library(processor-spc700 STATIC "src/processor/spc700/spc700.cpp")
add_library(processor-upd96050 STATIC "src/processor/upd96050/upd96050.cpp")
add_library(processor-wdc65816 STATIC "src/processor/wdc65816/wdc65816.cpp")

add_library(ruby STATIC ${ruby_hpp_files} "src/ruby/ruby.cpp")
target_link_libraries(ruby GL Xv asound openal pulse pulse-simple ao udev)

add_library(hiro-gtk2 STATIC ${hiro_hpp_files} "src/hiro/hiro.cpp")
target_link_libraries(hiro-gtk2 pthread cairo fontconfig freetype X11)
#gtksourceview-2.0 gtk-x11-2.0 gdk-x11-2.0 pangocairo-1.0 atk-1.0 gdk_pixbuf-2.0 gio-2.0 pangoft2-1.0 pango-1.0 gobject-2.0 glib-2.0)

#file(GLOB_RECURSE h_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
#file(GLOB_RECURSE c_files "${PROJECT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE hpp_files "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/target-bsnes/*.hpp")
file(GLOB_RECURSE cpp_files "${PROJECT_SOURCE_DIR}/src/target-bsnes*.cpp")

# Add Libraries/Executables
add_executable(${PROJECT_NAME} ${hpp_files} ${cpp_files})
target_link_libraries(${PROJECT_NAME} lzma gb emulator filter ruby hiro-gtk2)

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
