function(add_imgui)
	set(IMGUI_SOURCES
		libs/imgui/imconfig.h
		libs/imgui/imgui.cpp
		libs/imgui/imgui.h
		libs/imgui/imgui_demo.cpp
		libs/imgui/imgui_draw.cpp
		libs/imgui/imgui_internal.h
		libs/imgui/imgui_tables.cpp
		libs/imgui/imgui_widgets.cpp
		libs/imgui/imstb_rectpack.h
		libs/imgui/imstb_textedit.h
		libs/imgui/imstb_truetype.h
	)

	if(WIN32)
		list(APPEND IMGUI_SOURCES
			libs/imgui/backends/imgui_impl_opengl3.h
			libs/imgui/backends/imgui_impl_opengl3.cpp
			libs/imgui/backends/imgui_impl_win32.h
			libs/imgui/backends/imgui_impl_win32.cpp
		)
	endif()

	add_library(imgui ${IMGUI_SOURCES})

	target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
	target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends)

#	assign_source_group(${IMGUI_SOURCES})
endfunction()
