link_libraries(glibby)
include_directories(${GLIBBY_INCLUDE_DIR})

if(NOT ${GLIBBY_BUILD_RENDERER})
  message(
    FATAL_ERROR
      "GLIBBY_BUILD_RENDERER is OFF but you are attempting to build the examples. You cannot build examples without building the renderer."
  )
endif()

add_executable(distance distance.cpp)
add_executable(vk_window vk_window.cpp)
add_executable(jobs jobs.cpp)


file (GLOB_RECURSE RT_SHADERS CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vert" 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.comp"
    )

add_executable(compute_raytracer compute_raytracer/compute_raytracer.cpp ${RT_SHADERS})
# add_executable(...)

set(GUI_ONLY_BINARIES vk_window)

set(CONSOLE_BINARIES distance jobs compute_raytracer)

target_link_libraries(${GUI_ONLY_BINARIES} ${GLIBBY_RENDERER_NAME})
set_target_properties(${GUI_ONLY_BINARIES} ${CONSOLE_BINARIES}
                      PROPERTIES FOLDER "glibby/examples")

# SRC is with respect to ${CMAKE_CURRENT_SOURCE_DIR}
# DEST is with respect to ${CMAKE_BINARY_DIR}
file(COPY "assets" DESTINATION "./")
#file(COPY "compute_raytracer" DESTINATION "./")

find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

# custom command to compile shaders
foreach(GLSL ${RT_SHADERS})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})

  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)