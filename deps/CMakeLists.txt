if(GLIBBY_BUILD_RENDERER)
  add_subdirectory(glfw)
endif()

if(GLIBBY_BUILD_LANG_BINDINGS)
  add_subdirectory(pybind11)
endif()