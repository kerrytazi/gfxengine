if(WIN32)
	#set(CMAKE_GENERATOR_TOOLSET "ClangCL")
endif()

function(gfxengine_application target gfxengine_path sources)
	include(${CMAKE_CURRENT_SOURCE_DIR}/../gfxengine/cmake/gfxengine.cmake)

	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	add_executable(${target} ${sources})

	if(WIN32)
		set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE true)
	endif()

	add_subdirectory(${gfxengine_path} "${CMAKE_CURRENT_BINARY_DIR}/gfxengine")
	target_link_libraries(${target} gfxengine)

	assign_source_group(${PROJECT_SOURCES})
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${target})
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endfunction()
