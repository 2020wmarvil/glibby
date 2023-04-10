link_libraries(glibby)

pybind11_add_module(glibby python_bindings python/python_main.cpp)
target_include_directories(python_bindings PRIVATE ${GLIBBY_INCLUDE_DIR})
set_target_properties(python_bindings PROPERTIES FOLDER "glibby/bindings")