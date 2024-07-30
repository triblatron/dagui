CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT( nodefrontend )

FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(benchmark REQUIRED)
FIND_PACKAGE(Lua 5.4.6 EXACT REQUIRED)

SET( ALL_PUBLIC_HEADERS include/core/Container.h include/core/Rectangle.h include/core/Shape.h include/core/Widget.h )
SET( NODEFRONTEND_SRC src/core/Container.cpp src/core/Rectangle.cpp src/core/Shape.cpp src/core/Widget.cpp ${ALL_PUBLIC_HEADERS}
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

ADD_LIBRARY( nodefrontend SHARED ${NODEFRONTEND_SRC} )
TARGET_INCLUDE_DIRECTORIES( nodefrontend PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${LUA_INCLUDE_DIR})

set_target_properties( nodefrontend PROPERTIES DEFINE_SYMBOL NODEFRONTEND_LIBRARY )

add_subdirectory(src/apps/NodeFrontendTest)

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
INSTALL( TARGETS nodefrontend
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
