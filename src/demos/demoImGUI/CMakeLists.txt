ADD_EXECUTABLE(demoImGUI ../../../ThirdParty/imgui/imgui_demo.cpp ../../../ThirdParty/imgui/backends/imgui_impl_glfw.h ../../../ThirdParty/imgui/backends/imgui_impl_glfw.cpp ../../../ThirdParty/imgui/backends/imgui_impl_glfw.cpp ../../../ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
		dagEditor.cpp
		demoImGUI.cpp
)
MESSAGE("Freetype includes:" ${FREETYPE_INCLUDE_DIRS})
MESSAGE(${CMAKE_SOURCE_DIR}/include;${PROJECT_BINARY_DIR}/include;${LUA_INCLUDE_DIR})
MESSAGE("OpenGL includes:" ${OPENGL_INCLUDE_DIR})
MESSAGE("GLUT includes:" ${GLUT_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(demoImGUI PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imnodes;${CMAKE_SOURCE_DIR}/ThirdParty/imnodes/example;${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/ThirdParty/imgui;${PROJECT_BINARY_DIR}/include;${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
SET_TARGET_PROPERTIES( demoImGUI PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/lib" )
set_target_properties( demoImGUI PROPERTIES DEFINE_SYMBOL DAG_LIBRARY_STATIC )
TARGET_LINK_LIBRARIES(demoImGUI PRIVATE dagui dag dagbase ${LUA_LIBRARIES} GLEW::glew glfw OpenGL::GL OpenGL::GLU Freetype::Freetype)
INSTALL(TARGETS demoImGUI DESTINATION bin)
