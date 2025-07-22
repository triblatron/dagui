CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT( Dagui )

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
FIND_PACKAGE(OpenGL COMPONENTS OpenGL REQUIRED)
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(Lua 5.4 REQUIRED)
FIND_PACKAGE(glm REQUIRED)

SET( ALL_PUBLIC_HEADERS include/core/Circle.h include/core/Container.h include/core/Rectangle.h include/core/Shape.h include/core/Widget.h
        include/util/Completion.h
        include/util/CompletionTrie.h
        include/util/TrieNode.h
        include/util/CompletionSubstring.h
        include/core/CompositeShape.h
        include/core/Window.h
 		include/gfx/Image.h
        include/gfx/ImageSource.h
        include/gfx/FontImageSource.h
        include/gfx/TextureAtlas.h
        include/gfx/ImageDef.h
        include/gfx/GlyphImageDef.h
        include/gfx/BinImageDef.h
        include/core/SpaceTree.h
		include/core/BinPackingStrategy.h
		include/core/Atlas.h
		include/core/BinPackingStrategyFactory.h
		include/core/ShelfBinPackingStrategy.h
		include/core/MaxRectsBinPackingStrategy.h
		include/gfx/Renderer.h
		include/gfx/OpenGLRenderer.h
		include/core/Vec2f.h
		include/gfx/Mesh2D.h
		include/util/APIVersion.h
		include/gfx/RendererFactory.h
		include/gfx/OpenGL.h
		include/gfx/OpenGLMesh2D.h
		include/gfx/DrawingCommand.h
		include/gfx/AttributeDescriptor.h
		include/gfx/AttributeLayout.h
		include/gfx/ArrayDescriptor.h
		include/gfx/GenericMesh.h
		include/gfx/AttributeArray.h
		include/gfx/GenericAttributeArray.h
		include/gfx/Layer.h
		include/gfx/OpaqueAttributeArray.h
		include/util/glmOutput.h
		include/core/ValidatorNumber.h
		include/core/ValidatorRegex.h
		include/core/Validator.h
		include/core/ValidatorInteger.h
		include/core/ValidatorRange.h
		include/core/WidgetFactory.h
		include/core/Button.h
        include/core/Border.h
		include/core/Vertical.h
        include/core/Label.h
        include/core/Group.h
        include/core/SceneNodeFactory.h
        include/core/SceneNode.h
        include/core/WidgetPattern.h
        include/core/SceneTemplate.h
        include/core/Text.h
        include/core/WidgetSnippet.h
        include/core/ClipGroup.h
        include/core/WidgetToSceneNodeConverter.h
        include/core/LayoutProperties.h
        include/core/Constraint.h
        include/core/WidgetRef.h
        include/core/ResolveWidgetRefsVisitor.h
        include/core/ConstraintSolverVisitor.h
        include/core/LayoutConstraintsCollectionVisitor.h
        include/core/Style.h
        include/core/DrawCommandBuffer.h
        include/core/DrawCommand.h
        include/core/Batcher.h
        include/core/RenderBin.h
		include/core/Mesh.h
		include/core/ShapeFactory.h
		include/gfx/IndexArray.h
		include/gfx/OpaqueIndexArray.h
		include/gfx/MeshBackend.h
		include/gfx/OpenGLMesh.h
		include/core/GraphicsBackendFactory.h
		include/gfx/OpenGLBackendFactory.h
		include/core/Tessellation.h
)
SET( DAGUI_SRC src/core/Circle.cpp src/core/Container.cpp src/core/Rectangle.cpp src/core/Shape.cpp src/core/Widget.cpp ${ALL_PUBLIC_HEADERS}        
        src/util/CompletionTrie.cpp
        src/util/CompletionSubstring.cpp
        src/core/CompositeShape.cpp
        src/core/Window.cpp
        src/gfx/Image.cpp
        src/gfx/ImageSource.cpp
        src/gfx/FontImageSource.cpp
        src/gfx/TextureAtlas.cpp
        src/gfx/ImageDef.cpp
        src/gfx/GlyphImageDef.cpp
        src/gfx/BinImageDef.cpp
        src/core/SpaceTree.cpp
		src/core/BinPackingStrategyFactory.cpp
		src/core/ShelfBinPackingStrategy.cpp
		src/core/MaxRectsBinPackingStrategy.cpp
		src/gfx/OpenGLRenderer.cpp
		src/gfx/Mesh2D.cpp
		src/util/APIVersion.cpp
		src/gfx/RendererFactory.cpp
		src/gfx/OpenGL.cpp
		src/gfx/OpenGLMesh2D.cpp
		src/gfx/DrawingCommand.cpp
		src/gfx/GenericMesh.cpp
		src/gfx/AttributeDescriptor.cpp
		src/gfx/AttributeLayout.cpp
		src/gfx/ArrayDescriptor.cpp
		src/gfx/Layer.cpp
		src/gfx/OpaqueAttributeArray.cpp
		src/util/glmOutput.cpp
		src/core/ValidatorNumber.cpp
		src/core/ValidatorRegex.cpp
		src/core/Validator.cpp
		src/core/ValidatorInteger.cpp
		src/core/WidgetFactory.cpp
		include/core/RootWidget.h
		src/core/RootWidget.cpp
		src/core/Button.cpp
		src/core/Border.cpp
        src/core/Vertical.cpp
        src/core/Label.cpp
        src/core/Group.cpp
        src/core/SceneNodeFactory.cpp
        src/core/WidgetPattern.cpp
        src/core/SceneTemplate.cpp
        src/core/SceneNode.cpp
        src/core/Text.cpp
        src/core/WidgetSnippet.cpp
        src/core/ClipGroup.cpp
        src/core/WidgetToSceneNodeConverter.cpp
        src/core/LayoutProperties.cpp
        src/core/Constraint.cpp
        src/core/ResolveWidgetRefsVisitor.cpp
        src/core/ConstraintSolverVisitor.cpp
        src/core/LayoutConstraintsCollectionVisitor.cpp
        src/core/Style.cpp
        src/core/DrawCommandBuffer.cpp
        src/core/Batcher.cpp
        src/core/RenderBin.cpp
		src/core/ShapeFactory.cpp
		src/gfx/OpaqueIndexArray.cpp
		src/gfx/OpenGLMesh.cpp
		src/gfx/OpenGLBackendFactory.cpp
		src/core/Tessellation.cpp
)

SET(HEADERS_GROUP "Header Files")

SOURCE_GROUP(
        ${HEADERS_GROUP}
        FILES ${ALL_PUBLIC_HEADERS}
)

SET(GENERATED_HEADERS_GROUP "Generated header files")
SOURCE_GROUP(
        ${GENERATED_HEADERS_GROUP}
        FILES ${GENERATED_HEADERS}
)

SET( DEP_ROOT CACHE PATH "Dependency root" )

ADD_LIBRARY( dagui SHARED ${DAGUI_SRC} )
#INCLUDE_DIRECTORIES(PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
TARGET_INCLUDE_DIRECTORIES( dagui PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

set_target_properties( dagui PROPERTIES DEFINE_SYMBOL DAGUI_LIBRARY )
LINK_DIRECTORIES( ${DEP_ROOT}/lib )
TARGET_LINK_LIBRARIES( dagui PRIVATE dagbase ${LUA_LIBRARY} GLEW::glew GLUT::GLUT OpenGL::GL OpenGL::GLU Freetype::Freetype glm::glm )
#~ add_subdirectory(ThirdParty/dagbase)
add_subdirectory(ThirdParty/dag)
add_subdirectory(src/apps/DaguiTest)
add_subdirectory(src/apps/BenchmarkDagui)
add_subdirectory(src/demos/demoText)
add_subdirectory(src/demos/demoRenderer)

INCLUDE( CheckIncludeFile )
INCLUDE( CheckIncludeFileCXX )

CHECK_INCLUDE_FILE( windows.h HAVE_WINDOWS_H PATHS DOC "Whether we have the windows.h header, hopefully only on Windows!")
CHECK_INCLUDE_FILE( winsock2.h HAVE_WINSOCK2_H PATHS DOC "")
CHECK_INCLUDE_FILE( float.h HAVE_FLOAT_H  DOC "")
CHECK_INCLUDE_FILE( unistd.h HAVE_UNISTD_H  DOC "")
CHECK_INCLUDE_FILE( sys/types.h HAVE_SYS_TYPES_H  DOC "")
CHECK_INCLUDE_FILE( sys/socket.h HAVE_SYS_SOCKET_H  DOC "")
CHECK_INCLUDE_FILE( sys/time.h HAVE_SYS_TIME_H  DOC "")
CHECK_INCLUDE_FILE( sys/ioctl.h HAVE_SYS_IOCTL_H  DOC "")
CHECK_INCLUDE_FILE( sys/errno.h HAVE_SYS_ERRNO_H  DOC "")
CHECK_INCLUDE_FILE( sys/sysmp.h HAVE_SYS_SYSMP_H  DOC "")
CHECK_INCLUDE_FILE( sys/mman.h HAVE_SYS_MMAN_H  DOC "")
CHECK_INCLUDE_FILE( sys/stat.h HAVE_SYS_STAT_H  DOC "")
CHECK_INCLUDE_FILE( errno.h HAVE_ERRNO_H  DOC "")
CHECK_INCLUDE_FILE( netdb.h HAVE_NETDB_H  DOC "")
CHECK_INCLUDE_FILE( netinet/tcp.h HAVE_NETINET_TCP_H  DOC "")
CHECK_INCLUDE_FILE( netinet/in.h HAVE_NETINET_IN_H  DOC "")
CHECK_INCLUDE_FILE( arpa/inet.h HAVE_ARPA_INET_H  DOC "")
CHECK_INCLUDE_FILE( getopt.h HAVE_GETOPT_H  DOC "")
CHECK_INCLUDE_FILE( fcntl.h HAVE_FCNTL_H  DOC "")
CHECK_INCLUDE_FILE( stdint.h HAVE_STDINT_H  DOC "")
CHECK_INCLUDE_FILE( sys/timerfd.h HAVE_SYS_TIMERFD_H DOC "Whether we have the sys/timerfd header - only present on modern Linux distros, not on Control")
CHECK_INCLUDE_FILE( dirent.h HAVE_DIRENT_H DOC "Whether we have the dirent header, Linux and hopefully macOS X")
CHECK_INCLUDE_FILE( dlfcn.h HAVE_DLFCN_H DOC "Whether we have the dlfcn header for dynnamic libraries, Linux and hopefully macOS X")

CHECK_INCLUDE_FILE_CXX( iostream HAVE_STD_IOSTREAM DOC "")
CHECK_INCLUDE_FILE_CXX( fstream HAVE_STD_FSTREAM DOC "")
CHECK_INCLUDE_FILE_CXX( iosfwd HAVE_STD_IOSFWD DOC "")
CHECK_INCLUDE_FILE_CXX( cassert HAVE_STD_CASSERT DOC "")
CHECK_INCLUDE_FILE_CXX( sstream HAVE_STD_SSTREAM DOC "")
CHECK_INCLUDE_FILE_CXX( stdexcept HAVE_STD_EXCEPT )

CONFIGURE_FILE( include/config/config.h.in ${PROJECT_BINARY_DIR}/include/config/config.h )
IF ( UNIX AND NOT APPLE )
    CONFIGURE_FILE( etc/setup.sh.in ${PROJECT_BINARY_DIR}/etc/setup.sh )
ELSEIF ( APPLE )
    CONFIGURE_FILE( etc/setup_macos.sh.in ${PROJECT_BINARY_DIR}/etc/setup.sh )
ELSEIF ( WIN32 )
    CONFIGURE_FILE( etc/setup.bat.in ${PROJECT_BINARY_DIR}/etc/setup.bat )
ENDIF ( UNIX AND NOT APPLE )
IF ( UNIX )
    INSTALL( FILES ${PROJECT_BINARY_DIR}/etc/setup.sh DESTINATION . )
ELSEIF ( WIN32 )
    INSTALL( FILES ${PROJECT_BINARY_DIR}/etc/setup.bat DESTINATION . )
ENDIF( UNIX )

MESSAGE("glm found:" ${glm_FOUND})
MESSAGE("glm includes:" ${glm_INCLUDE_DIR})
INSTALL( TARGETS dagui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
INSTALL(DIRECTORY data DESTINATION .)
INSTALL(DIRECTORY etc DESTINATION .)