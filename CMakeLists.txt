cmake_minimum_required(VERSION 3.25)

include(cmake/assign_source_group.cmake)
include(cmake/imgui.cmake)

project(gfxengine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags
set(GFXENGINE_USE_GLFW OFF)
set(GFXENGINE_EDITOR ON)

set(PROJECT_SOURCES
	cmake/assign_source_group.cmake
	cmake/gfxengine.cmake
	cmake/imgui.cmake

	src/include/gfxengine/base_application.hpp
	src/include/gfxengine/frame.hpp
	src/include/gfxengine/graphics.hpp
	src/include/gfxengine/input_controller.hpp
	src/include/gfxengine/math.hpp
	src/include/gfxengine/platform.hpp
	src/include/gfxengine/window.hpp
	src/include/gfxengine/window_event_handler.hpp

	src/private/my_windows.hpp
	src/private/wglext.h

	src/base_application.cpp
	src/frame.cpp
	src/graphics.cpp
	src/main.cpp
	src/platform.cpp
	src/window.cpp
)

add_library(gfxengine ${PROJECT_SOURCES})

target_compile_definitions(gfxengine PUBLIC _ITERATOR_DEBUG_LEVEL=0)

if (MSVC AND (MSVC_VERSION GREATER_EQUAL 1914))
	target_compile_options(gfxengine INTERFACE "/Zc:__cplusplus")
endif()

if(WIN32)
	target_compile_definitions(gfxengine PRIVATE GFXENGINE_PLATFORM_WINDOWS=1)
endif()

target_include_directories(gfxengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(gfxengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

assign_source_group(${PROJECT_SOURCES})

add_subdirectory(libs/glad)
target_link_libraries(gfxengine glad)
target_include_directories(gfxengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)

if(GFXENGINE_USE_GLFW)
	add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)
	add_dependencies(gfxengine glfw)
	target_link_libraries(gfxengine glfw)

	target_compile_definitions(gfxengine PUBLIC GFXENGINE_USE_GLFW=1)
endif()

if(GFXENGINE_EDITOR)
	add_imgui()
	target_link_libraries(gfxengine imgui)

	target_compile_definitions(gfxengine PUBLIC GFXENGINE_EDITOR=1)
endif()


add_subdirectory(libs/glm)
target_link_libraries(gfxengine glm)
