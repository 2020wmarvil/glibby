link_libraries(glibby)
include_directories(${GLIBBY_INCLUDE_DIR})

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