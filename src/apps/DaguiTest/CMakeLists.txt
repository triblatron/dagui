SET( DaguiTest_SRC DaguiTest.cpp RoundTripTest.cpp
        GraphicsTest.cpp
        ValidationTest.cpp
        WidgetTest.cpp
        ../../../ThirdParty/dag/thirdparty/dagbase/include/test/TestUtils.h
        ../../../ThirdParty/dag/thirdparty/dagbase/src/test/TestUtils.cpp
        PatternMatchingTest.cpp
        SceneNodeTest.cpp)
ADD_EXECUTABLE( DaguiTest ${DaguiTest_SRC} )
IF (UNIX)
LIST(APPEND LUA_LIBRARIES dl)
ENDIF(UNIX)
TARGET_LINK_LIBRARIES( DaguiTest PRIVATE dagbase ${LUA_LIBRARIES} Freetype::Freetype;dagui;GTest::gtest_main;GTest::gmock;GLUT::GLUT)
INSTALL(TARGETS DaguiTest RUNTIME DESTINATION bin)
SET_TARGET_PROPERTIES( DaguiTest PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/lib" )
