file(GLOB_RECURSE GLIBBY_SOURCES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GLIBBY_HEADERS CONFIGURE_DEPENDS ${GLIBBY_INCLUDE_DIR}/*.h)
file(GLOB_RECURSE GLIBBY_RENDERER_SOURCES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/renderer/*.cpp)
file(GLOB_RECURSE GLIBBY_RENDERER_HEADERS CONFIGURE_DEPENDS
     ${GLIBBY_RENDERER_INCLUDE_DIR}/*.h)

foreach(_rendererFile ${GLIBBY_RENDERER_SOURCES})
  list(REMOVE_ITEM GLIBBY_SOURCES ${_rendererFile})
endforeach()

foreach(_rendererFile ${GLIBBY_RENDERER_HEADERS})
  list(REMOVE_ITEM GLIBBY_HEADERS ${_rendererFile})
endforeach()

# adapted from
# https://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake
function(assign_source_group)
  foreach(_source IN ITEMS ${ARGN})
    if(IS_ABSOLUTE "${_source}")
      file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
    else()
      set(_source_rel "${_source}")
    endif()
    get_filename_component(_source_path "${_source_rel}" PATH)

    string(REPLACE "../include" "" _trimmed_source_path "${_source_path}"
    )# trim away the relative path
    string(REPLACE "/" "\\" _source_path_msvc "${_trimmed_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
  endforeach()
endfunction(assign_source_group)

assign_source_group(${GLIBBY_SOURCES})
assign_source_group(${GLIBBY_HEADERS})

add_library(${PROJECT_NAME} ${LIB_TYPE} ${GLIBBY_SOURCES} ${GLIBBY_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "glibby")

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(${PROJECT_NAME}
                           PRIVATE $<$<CONFIG:Debug>:GLIBBY_DEBUG>)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLIBBY_INCLUDE_DIR})

if(GLIBBY_BUILD_RENDERER)
  find_package(Vulkan)
  if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
  else()
    message(STATUS ${Vulkan_LIBRARY})
  endif()

  assign_source_group(${GLIBBY_RENDERER_SOURCES})
  assign_source_group(${GLIBBY_RENDERER_HEADERS})

  add_library(${GLIBBY_RENDERER_NAME} ${LIB_TYPE} ${GLIBBY_RENDERER_SOURCES}
                                      ${GLIBBY_RENDERER_HEADERS})
  set_target_properties(${GLIBBY_RENDERER_NAME} PROPERTIES FOLDER "glibby")

  if(MSVC)
    target_compile_options(${GLIBBY_RENDERER_NAME} PRIVATE /W4)
  else()
    target_compile_options(${GLIBBY_RENDERER_NAME} PRIVATE -Wall -Wextra
                                                           -Wpedantic)
  endif()

  target_compile_definitions(${GLIBBY_RENDERER_NAME}
                             PRIVATE $<$<CONFIG:Debug>:GLIBBY_DEBUG>)
  target_include_directories(
    ${GLIBBY_RENDERER_NAME}
    PRIVATE ${GLIBBY_RENDERER_INCLUDE_DIR} ${Vulkan_INCLUDE_DIRS}
            ${CMAKE_SOURCE_DIR}/deps/glfw/include ${CMAKE_SOURCE_DIR}/deps/glm ${CMAKE_SOURCE_DIR}/deps/stb_image)
  target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARY} glfw glm)
endif()
