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
# add_executable(...)

set(GUI_ONLY_BINARIES vk_window)

set(CONSOLE_BINARIES distance jobs)

target_link_libraries(${GUI_ONLY_BINARIES} ${GLIBBY_RENDERER_NAME})
set_target_properties(${GUI_ONLY_BINARIES} ${CONSOLE_BINARIES}
                      PROPERTIES FOLDER "glibby/examples")

# SRC is with respect to ${CMAKE_CURRENT_SOURCE_DIR}
# DEST is with respect to ${CMAKE_BINARY_DIR}
file(COPY "assets" DESTINATION "./")