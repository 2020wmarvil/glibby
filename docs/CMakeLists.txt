# NOTE: The order of this list determines the order of items in the Guides
#       (i.e. Pages) list in the generated documentation
set(doxy_files
#    main.dox
#    news.dox
#    quick.dox
#    moving.dox
#    compile.dox
#    build.dox
#    intro.dox
#    context.dox
#    monitor.dox
#    window.dox
#    input.dox
#    vulkan.dox
#    compat.dox
#    internal.dox
)

#set(extra_files DoxygenLayout.xml header.html footer.html extra.css spaces.svg)

file (GLOB_RECURSE GLIBBY_HEADERS_FOR_DOCS CONFIGURE_DEPENDS ${GLIBBY_INCLUDE_DIR}/*.h)

# Format the source list into a Doxyfile INPUT value that Doxygen can parse
foreach(path IN LISTS header_paths)
    string(APPEND GLIBBY_DOXYGEN_INPUT " \\\n\"${path}\"")
endforeach()
foreach(file IN LISTS doxy_files)
    string(APPEND GLIBBY_DOXYGEN_INPUT " \\\n\"${CMAKE_CURRENT_SOURCE_DIR}/${file}\"")
endforeach()

configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_command(OUTPUT "html/index.html"
                   COMMAND "${DOXYGEN_EXECUTABLE}"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                   MAIN_DEPENDENCY Doxyfile
                   DEPENDS ${header_paths} ${doxy_files} #${extra_files}
                   COMMENT "Generating HTML documentation"
                   VERBATIM)

add_custom_target(glibby_docs ALL SOURCES "html/index.html")
set_target_properties(glibby_docs PROPERTIES FOLDER "glibby")