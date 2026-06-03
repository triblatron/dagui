ADD_EXECUTABLE(demoImGUI ../../../ThirdParty/imgui/imgui_demo.cpp ../../../ThirdParty/imgui/backends/imgui_impl_glfw.h ../../../ThirdParty/imgui/backends/imgui_impl_glfw.cpp ../../../ThirdParty/imgui/backends/imgui_impl_glfw.cpp ../../../ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
		../../../ThirdParty/imgui/imconfig.h
		../../../ThirdParty/imgui/imgui.h
		../../../ThirdParty/imgui/imgui_internal.h
		../../../ThirdParty/imgui/imstb_rectpack.h
		../../../ThirdParty/imgui/imstb_textedit.h
		../../../ThirdParty/imgui/imstb_truetype.h
		../../../ThirdParty/imgui/imgui.cpp
		../../../ThirdParty/imgui/imgui_draw.cpp
		../../../ThirdParty/imgui/imgui_tables.cpp
		../../../ThirdParty/imgui/imgui_widgets.cpp
		demoImGUI.cpp
)
MESSAGE("Freetype includes:" ${FREETYPE_INCLUDE_DIRS})
MESSAGE(${CMAKE_SOURCE_DIR}/include;${PROJECT_BINARY_DIR}/include;${LUA_INCLUDE_DIR})
MESSAGE("OpenGL includes:" ${OPENGL_INCLUDE_DIR})
MESSAGE("GLUT includes:" ${GLUT_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(demoImGUI PUBLIC ${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/ThirdParty/imgui;${PROJECT_BINARY_DIR}/include;${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
SET_TARGET_PROPERTIES( demoImGUI PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/lib" )
TARGET_LINK_LIBRARIES(demoImGUI PRIVATE GLEW::glew glfw OpenGL::GL OpenGL::GLU Freetype::Freetype)
INSTALL(TARGETS demoImGUI DESTINATION bin)
