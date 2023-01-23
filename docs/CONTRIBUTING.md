# Contributing

## Folder Structure

- Source files go in ```src/```
- Header files go in ```include/glibby/```
- Renderer specific source/header files go in ```src/renderer/``` and ```include/glibby/renderer/``` respectively
- External dependencies go in ```deps/```
- Examples go in ```examples/```
- Tests go in ```tests/```
- Documentation files go in ```docs/```
- Scripts go in ```scripts/```

File hierarchy in ```src/``` and ```include/glibby``` should be mirrored. 

## Pull Request Process

If any steps are missing from this process, notify a maintainer, make an issue, or create a PR!

### Prerequisites

Install the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/), cmake, and doxygen.

#### Windows (using [Chocolatey](https://chocolatey.org/))

```
choco install cmake
choco install doxygen
```

#### Unix

```
sudo apt install cmake
sudo apt install doxygen
```

### Creating a Feature

1. Fork the project
2. Create a feature branch (```git checkout -b your-feature```)
3. Implement your feature (```git commit -m "Implement feature" && git push origin your-feature```)
4. Ensure your work aligns with our [Coding Standards](https://github.com/2020wmarvil/glibby/blob/main/CodingStandard.md)
5. Ensure all tests are still passing (```./glibby_test_suite --order rand --warn NoAssertions```)
6. Merge from main (```git merge upstream/main```)
7. Create a pull request and complete code review process

Commit messages should be in the imperative mood, e.g. "Implement 2D curve intersection solver" as opposed to "2D curve intersection solver".

### Generating the Project

```
cmake -E make_directory build
cd build
cmake .. -G <generator> -DCMAKE_BUILD_TYPE=Debug -DGLIBBY_BUILD_RENDERER=ON -DGLIBBY_BUILD_EXAMPLES=ON -DGLIBBY_BUILD_DOCS=ON
```

Common generators are ```unix makefiles``` for Unix or ```"Visual Studio 16 2019```/```Visual Studio 17 2022``` for Windows. If you're using Windows, this will generate an MSBuild solution that you can open in an IDE of your choice (Visual Studio, Rider, CLion, etc.).

Here is a list of built-in [CMake Generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).

### Building the Project

Building the project on its own will not do much for you, since it exports as a ```.lib``` file. In order to develop our project, we will build against examples and tests. This serves a dual purpose, as now our code will have higher test coverage and examples/templates will exist for our users.

#### From the Terminal/Powershell

```
cd build
cmake --build . --config Debug --target <target>
./<path_to_target>/target
```

Our project generates several build targets, which you can choose from here:
- ```ALL_BUILD```
- ```<example_name>```
- ```<test_name>```
- ```glibby_test_suite``` or ```RUN_TESTS```
- ```glibby_docs```

#### From Visual Studio

If you would like to build from Visual Studio instead, you can select the build target by right-clicking on the desired target and selecting 'Set as Startup Project'.

images go here!

## Adding New Files

Adding source and header files is straightforward. For each new header you provide in ```include/glibby/<path_to_header>``` there should be a corresponding source file in ```src/<path_to_source>```. You should re-run the cmake generator step whenever you add new files, since the globs cannot detect new files automatically.

### Adding Examples

When you are creating a new example, create a source file under ```examples/```. 

Example source files are not globbed because we need to individually mark them as console-only or GUI-only. This is necessary because GUI-only source files must link against the rendering engine. 

Modify ```examples/CMakeLists.txt``` so that it detects your new source file:
```CMake
add_executable(distance distance.cpp)
add_executable(vk_window vk_window.cpp)
...
add_executable(your_example_name your_example_name.cpp) # you add this line

# Add your new example to either GUI_ONLY_BINARIES or CONSOLE_BINARIES depending on if it needs the rendering engine
set(GUI_ONLY_BINARIES vk_window example_name)
set(CONSOLE_BINARIES distance)
```

Don't forget to re-run the CMake generator step.

### Adding Tests

There should be a test source file for ever subdirectory in glibby. For example:
- ```src/primitives``` -> ```tests/primitives_tests.cpp```
- ```src/convex_hull``` -> ```tests/convex_hull_tests.cpp```
- etc.

We use Catch2 for our unit testing suite, so refer to the [Catch2 docs](https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md) for specific use cases.

Example unit test:
```C++
TEST_CASE("Point2D distance", "[primitive][point2D]") {
    glibby::Point2D p1 = { 0.0f, 0.0f };
    glibby::Point2D p2 = { 5.0f, 15.0f };

    CHECK(glibby::Distance(p1, p1) == 0);
    CHECK_FALSE(glibby::Distance(p1, p2) == 0);
    CHECK_THAT(glibby::Distance(p1, p2), Catch::Matchers::WithinAbs(15.81139f, FLT_NEAR_ZERO));
}
```

Example benchmark:
```C++
TEST_CASE("Point2D distance", "[!benchmark][primitive][point2D]") {
    BENCHMARK("Point2D distance") {
        return glibby::Distance({ 0.0f, 0.0f }, { 5.0f, 8.0f });
    };
}
```

#### Running Tests

To execute all tests:
```
./glibby_test_suite --order rand --warn NoAssertions
```

To execute by name or tag with options:
```
./glibby_test_suite <test_name> <[tag1][tag2][...]> <options>
```

Helpful options are:
- ```--skip-benchmarks``` to skip benchmark tests (which are very slow)
- ```--break``` to enter the debugger when an exception is thrown
- for more see [Catch2 commandline](https://github.com/catchorg/Catch2/blob/devel/docs/command-line.md#top)

Runs the test named Point2D distance:
```
./glibby_test_suite "Point2D distance" --skip-benchmarks
```

Runs all tests tagged primitive and point2D:
```
./glibby_test_suite [primitive][point2D] --skip-benchmarks
```

Don't forget to re-run the CMake generator step.

### Adding Docs

TODO: Section on using doxygen comments and adding doxygen files

You don't need to generate the docs locally since they are deployed automatically to our GitHub Pages through an Actions workflow. However, if you want to generate them anyways, build the target ```glibby_docs``` and open the generated HTML file at ```build/docs/html/index.html```. This is useful if you're making large changes to the docpages and need to verify your changes are correct before pushing. 

### Modifying the Build System

If you make edits to the CMake system, make sure to run the platform-appropriate ```cmake-format``` located in ```scripts/```.

## Code of Conduct

glibby follows the [Contributor Covenant](http://contributor-covenant.org/version/1/3/0/) Code of Conduct.
