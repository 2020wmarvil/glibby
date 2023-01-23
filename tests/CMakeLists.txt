include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

set_target_properties(Catch2;Catch2WithMain PROPERTIES FOLDER "Catch2")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(glibby_test_suite ${TEST_SOURCES})
set_target_properties(glibby_test_suite PROPERTIES FOLDER "glibby/tests")
target_include_directories(glibby_test_suite PRIVATE ${GLIBBY_INCLUDE_DIR})
target_link_libraries(glibby_test_suite PRIVATE glibby Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(glibby_test_suite)
