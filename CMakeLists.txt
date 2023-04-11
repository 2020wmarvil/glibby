cmake_minimum_required(VERSION 3.20)

project(glibby VERSION 0.0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(GLIBBY_BUILD_RENDERER "Builds companion renderer library" ON)
option(GLIBBY_BUILD_TESTS "Builds test suite" ON)
option(GLIBBY_BUILD_EXAMPLES "Builds example applications" ON)
option(GLIBBY_BUILD_DOCS "Builds documentation" ON)
option(GLIBBY_BUILD_LANG_BINDINGS "Builds language bindings" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_TYPE STATIC)
set(GLIBBY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(GLIBBY_RENDERER_NAME "glibby_renderer")
set(GLIBBY_RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/glibby/renderer)

if(GLIBBY_BUILD_DOCS)
  set(DOXYGEN_SKIP_DOT TRUE)
  find_package(Doxygen)
endif()

add_subdirectory(deps)
add_subdirectory(src)

if(GLIBBY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(GLIBBY_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(DOXYGEN_FOUND AND GLIBBY_BUILD_DOCS)
  add_subdirectory(docs)
endif()

if(GLIBBY_BUILD_LANG_BINDINGS)
  add_subdirectory(bindings)
endif()
