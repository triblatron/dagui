ADD_EXECUTABLE(demoRenderer demoRenderer.cpp)
MESSAGE("Freetype includes:" ${FREETYPE_INCLUDE_DIRS})
MESSAGE(${CMAKE_SOURCE_DIR}/include;${PROJECT_BINARY_DIR}/include;${LUA_INCLUDE_DIR})
MESSAGE("OpenGL includes:" ${OPENGL_INCLUDE_DIR})
MESSAGE("GLUT includes:" ${GLUT_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(demoRenderer PUBLIC ${CMAKE_SOURCE_DIR}/include;${PROJECT_BINARY_DIR}/include;${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
SET_TARGET_PROPERTIES( demoRenderer PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/lib" )
TARGET_LINK_LIBRARIES(demoRenderer PRIVATE dagui dagbase ${LUA_LIBRARIES} GLEW::glew glfw OpenGL::GL OpenGL::GLU Freetype::Freetype)
INSTALL(TARGETS demoRenderer DESTINATION bin)
