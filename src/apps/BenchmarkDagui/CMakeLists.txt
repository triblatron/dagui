ADD_EXECUTABLE( BenchmarkDagui BenchmarkDagui.cpp )
TARGET_INCLUDE_DIRECTORIES( BenchmarkDagui PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include )
message("benchmark_INCLUDE_DIRS: ${benchmark_INCLUDE_DIRS}")
SET(BENCHMARK_LIBS ${benchmark_LIBRARY})
# IF (WIN32)
	# SET(BENCHMARK_LIBS ${BENCHMARK_LIBS} Shlwapi)
	# target_compile_definitions(BenchmarkDagui PUBLIC BENCHMARK_STATIC_DEFINE)
# ENDIF(WIN32)
target_link_libraries(BenchmarkDagui PRIVATE benchmark::benchmark dagui)
INSTALL(TARGETS BenchmarkDagui RUNTIME DESTINATION bin)
