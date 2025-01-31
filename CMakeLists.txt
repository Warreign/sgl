# Min. CMake version.
cmake_minimum_required(VERSION 3.11)

# Project name.
set(NAME supersglapp)

# A name of the project.
project(${NAME})

# GLUT.
find_package(GLUT)

# OpenGL.
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL)

option(DEBUG_MSG "Print debug messages") 
option(USE_SIMD "Enable usage of SIMD instructions for matrix multiplication" ON)

# Set general compiler flags.
if (WIN32)
  add_definitions(-D__WINDOWS__ -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3 -mavx2 -Wall -Wno-unknown-pragmas -Wno-unused-function")
endif (WIN32)

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -framework GLUT")
endif (APPLE)

# Do not use GUI by default.
add_definitions(-DUSE_GUI=1)

set(HDR
  hdrloader.h
  nffread.h
  nffstore.h
  nffwrite.h
  timer.h
)

set(SRC
  hdrloader.cpp
  nffread.cpp
  testapp.cpp
  timer.cpp
)

source_group("Header Files\\" FILES ${HDR})
source_group("Source Files\\" FILES ${SRC})

# Add all files to excutable.
add_executable(${NAME}
  ${HDR}
  ${SRC}
)

# Use freeglut on Windows.
if (WIN32)
  if (NOT GLUT_FOUND)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLUT_LIB_PATH freeglut/lib/x64)
    add_custom_command(TARGET ${NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/freeglut/bin/x64/freeglut.dll ${CMAKE_SOURCE_DIR}/.
    )
  else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLUT_LIB_PATH freeglut/lib)
    add_custom_command(TARGET ${NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/freeglut/bin/freeglut.dll ${CMAKE_SOURCE_DIR}/.
    )
  endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  find_library(GLUT_LIBRARIES
    NAMES freeglut
    PATHS
    ${GLUT_LIB_PATH}
    DOC "The GLUT library"
  )
  find_path(GLUT_INCLUDE_DIR GL/glut.h
    freeglut/include
    DOC "The directory where GL/glut.h resides"
  )
  include_directories(${GLUT_INCLUDE_DIR})
  endif (NOT GLUT_FOUND)
endif (WIN32)

if(SGL_BACKEND STREQUAL "rust")
  message(STATUS "Using Rust backend")
  add_subdirectory(sgl-rust)
elseif(SGL_BACKEND STREQUAL "zig")
  message(STATUS "Using Zig backend")
  add_subdirectory(sgl-zig)
else()
  message(STATUS "Using C++ backend")
  add_subdirectory(sgl-cpp)
endif()

# Link libraries.
target_link_libraries(${NAME} sgl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

# Set output directory.
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

target_compile_definitions(sgl
  PUBLIC
    SGL_TEXTURES_ENABLED
    SGL_ANTIALIASING_ENABLED
    )

target_compile_definitions(${NAME} 
  PRIVATE
    # TEST0
    # TEST1
    # TEST2
    # TEST3
    # TEST4
    TEST5
    )

# Properties.
set_target_properties(${NAME} PROPERTIES
  DEBUG_OUTPUT_NAME ${NAME}_d
  RELEASE_OUTPUT_NAME ${NAME}
  RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
)

install(TARGETS ${NAME}
  RUNTIME DESTINATION ${BIN_DIR}
)

enable_testing()
add_subdirectory(tests)
