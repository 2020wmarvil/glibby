link_libraries(glibby)
include_directories(${GLIBBY_INCLUDE_DIR})

if(NOT ${GLIBBY_BUILD_RENDERER})
  message(FATAL_ERROR "GLIBBY_BUILD_RENDERER is OFF but you are attempting to build the examples. You cannot build examples without building the renderer.")
endif()

add_executable(distance distance.cpp)
add_executable(vk_window vk_window.cpp)
#add_executable(...)

set(GUI_ONLY_BINARIES 
    vk_window
)

set(CONSOLE_BINARIES 
    distance
)

target_link_libraries(${GUI_ONLY_BINARIES} ${GLIBBY_RENDERER_NAME})
set_target_properties(${GUI_ONLY_BINARIES} ${CONSOLE_BINARIES} PROPERTIES FOLDER "glibby/examples")