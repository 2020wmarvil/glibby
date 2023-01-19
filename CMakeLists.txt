cmake_minimum_required(VERSION 3.20)

project(glibby VERSION 0.0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
#option(ENABLE_CTEST "Enables testing" ON)
option(GLIBBY_BUILD_RENDERER "Builds companion renderer library" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_TYPE STATIC)
if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
endif()

set(GLIBBY_BUILD_EXAMPLES 1)
set(GLIBBY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(GLIBBY_RENDERER_NAME "glibby_renderer")
set(GLIBBY_RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/glibby/renderer)

add_subdirectory(deps)
add_subdirectory(src)

if (GLIBBY_BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()

if (GLIBBY_BUILD_TESTS)
    include(CTest)
	add_subdirectory(tests)
endif()

if (DOXYGEN_FOUND AND GLIBBY_BUILD_DOCS)
	add_subdirectory(docs)
endif()