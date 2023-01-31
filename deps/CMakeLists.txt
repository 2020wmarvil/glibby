if(GLIBBY_BUILD_RENDERER)
  add_subdirectory(glfw) # TODO: move GLFW to a submodule or fetch_content
  add_subdirectory(glm) # TODO: remove glm dependency
endif()
