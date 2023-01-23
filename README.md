# glibby

<!-- ![banner]() -->

[![build health](https://github.com/2020wmarvil/glibby/actions/workflows/cmake.yml/badge.svg)](https://github.com/2020wmarvil/glibby/actions)
[![license](https://img.shields.io/github/license/2020wmarvil/glibby.svg)](LICENSE)
[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

The goal of this project is to create a C++ library encapsulating both classical and experimental techniques in computational geometry, with a focus on real-time performance and ease of use.

Visit our [documentation pages](https://2020wmarvil.github.io/glibby/docs) for more information on how to build and use glibby in your projects. 

[Spring 2023 Pitch Deck.](https://docs.google.com/presentation/d/1FOssrwILgcFnzoFG_aA9-h7bjptdw4Wtrp3-SIhfmjk/edit#slide=id.p)

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [High-Level Todos](#high-level-todos)
- [Contributing](#contributing)
- [Maintainers](#maintainers)
- [License](#license)

## Installation

```
git clone https://github.com/2020wmarvil/glibby.git
cmake -E make_directory build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DGLIBBY_BUILD_RENDERER=ON -DGLIBBY_BUILD_EXAMPLES=ON -DGLIBBY_BUILD_DOCS=OFF
make install -j8
```

Note, to build with the debug renderer, you will need the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) installed.

## Usage

```C++
#include "glibby/primitives/point2D.h"

int main() {
    glibby::Distance({ 0.0f, 0.0f }, { 5.0f, 5.0f });
}
```

## Examples

Coming soon...

## High-Level Todos
- [ ] Core Geometry Library
  - [ ] Math Library
  - [ ] Primitive Shapes and Operations
  - [ ] Surface Mesh Data Structures
  - [ ] Volumetric Mesh Data Structures
  - [ ] Surface/Volumetric Mesh Operations
  - [ ] Triangulation/Tetrahedralization
  - [ ] Voronoi Diagrams
  - [ ] Arrangements
  - [ ] Surface/Shape Reconstruction
  - [ ] Spatial Algorithms
  - [ ] Convex Hulls
- [ ] Job Dispatch
- [ ] Compute Dispatch
- [ ] Companion Renderer
  - [ ] Vulkan (in progress)
  - [ ] OpenGL, DirectX, Metal
- [ ] Language Bindings
  - [ ] Python, C#, Rust, ...

## Contributing

Feel free to dive in! [Open an issue](https://github.com/2020wmarvil/glibby/issues/new) or submit PRs. Check out [Contributing](https://github.com/2020wmarvil/glibby/blob/main/docs/CONTRIBUTING.md) to see how to contribute.

glibby follows the [Contributor Covenant](http://contributor-covenant.org/version/1/3/0/) Code of Conduct.

### Contributors

This project exists thanks to all the people who contribute. 

<!-- <a href="https://github.com/2020wmarvil/glibby/graphs/contributors"><img src="https://opencollective.com/glibby/contributors.svg?width=890&button=false" /></a> -->

## Maintainers

[@Wyatt Marvil](https://github.com/2020wmarvil). Reach me at 2020wmarvil@gmail.com or join our [Discord server](https://discord.gg/jTcSZxsZ34).

## License

[MIT (c) Wyatt Marvil](LICENSE)
