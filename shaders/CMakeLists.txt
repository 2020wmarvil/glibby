find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

file (GLOB_RECURSE SHADERS CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vert" 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.comp"
    )

# custom command to compile shaders
foreach(GLSL ${SHADERS})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})

  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

# create a shader target
add_custom_target(
    shaders
    SOURCES ${SHADERS}
    DEPENDS ${SPIRV_BINARY_FILES}
    )

set_target_properties(shaders PROPERTIES FOLDER "glibby")

foreach(GLSL ${SHADERS})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SRC "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  set(DEST "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    TARGET shaders
    COMMAND ${CMAKE_COMMAND} -E copy ${SRC} "${DEST}"
    )
endforeach(GLSL)