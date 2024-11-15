project(sgl)
cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE SGL_CXX "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SGL_CXX})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (USE_SIMD)
  target_compile_definitions(${PROJECT_NAME} PRIVATE SGL_SIMD)
endif()

if (DEBUG_MSG)
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MSG)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${SGL_CXX})

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
)
