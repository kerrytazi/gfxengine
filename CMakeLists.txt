cmake_minimum_required(VERSION 3.25)

include(cmake/assign_source_group.cmake)
include(cmake/imgui.cmake)

project(gfxengine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags
set(GFXENGINE_EDITOR ON)

set(PROJECT_SOURCES
	cmake/assign_source_group.cmake
	cmake/gfxengine.cmake
	cmake/imgui.cmake

	src/include/gfxengine/buffered_cstr.hpp
	src/include/gfxengine/base_application.hpp
	src/include/gfxengine/file.hpp
	src/include/gfxengine/frame.hpp
	src/include/gfxengine/graphics.hpp
	src/include/gfxengine/image.hpp
	src/include/gfxengine/input_controller.hpp
	src/include/gfxengine/logger.hpp
	src/include/gfxengine/math.hpp
	src/include/gfxengine/noise_generator.hpp
	src/include/gfxengine/platform.hpp
	src/include/gfxengine/window.hpp
	src/include/gfxengine/window_event_handler.hpp

	src/private/my_windows.hpp
	src/private/wglext.h

	src/base_application.cpp
	src/file.cpp
	src/frame.cpp
	src/graphics.cpp
	src/image.cpp
	src/logger.cpp
	src/main.cpp
	src/noise_generator.cpp
	src/platform.cpp
	src/window.cpp
)

add_library(gfxengine ${PROJECT_SOURCES})

target_compile_definitions(gfxengine PUBLIC _ITERATOR_DEBUG_LEVEL=0)

if (WIN32)
	target_compile_definitions(gfxengine PRIVATE GFXENGINE_PLATFORM_WINDOWS=1)
endif()

target_include_directories(gfxengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(gfxengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

assign_source_group(${PROJECT_SOURCES})

add_subdirectory(libs/glad)
target_link_libraries(gfxengine glad)
target_include_directories(gfxengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)
set_target_properties(glad PROPERTIES FOLDER libs)

if (GFXENGINE_EDITOR)
	add_imgui()
	target_link_libraries(gfxengine imgui)
	set_target_properties(imgui PROPERTIES FOLDER libs)

	target_compile_definitions(gfxengine PUBLIC GFXENGINE_EDITOR=1)
endif()

add_subdirectory(libs/zlib EXCLUDE_FROM_ALL)
target_link_libraries(gfxengine zlibstatic)
set_target_properties(zlibstatic PROPERTIES FOLDER libs)

# let libpng use our zlib
set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/libs/zlib/Debug/zlibstaticd.lib)
set(ZLIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib)

add_subdirectory(libs/libpng EXCLUDE_FROM_ALL)
target_link_libraries(gfxengine png_static)
target_include_directories(png_static PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libs/zlib)
set_target_properties(png_static PROPERTIES FOLDER libs)
set_target_properties(png_genfiles PROPERTIES FOLDER libs)
