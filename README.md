# glibby

The goal of this project is to create a library encapsulating both classical and experimental techniques in computational geometry, with a focus on real-time performance and ease of use.

## High-Level Todos
- [ ] Build System (in progress)
  - [ ] Cross-platform CMake generation for Unix makefiles and MSBuild solutions
  - [ ] Unit testing suite
  - [ ] GitHub Actions build health
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

## Usage

For now, you will need the Vulkan SDK installed in order to build the demos/tests.

### Linux and MacOS

```
chmod +x ./setup_unix.sh ./build_unix.sh
```

```
./setup_unix.sh
```

```
./build_unix.sh
```

### Windows

```
./setup_vs2019.bat
```

Build files will be generated under ProjectFiles/

## Demos (todo)

## Contributing

1. Fork the project
2. Create a feature branch (```git checkout -b feature/YourFeature```)
3. Implement your feature (``` git commit -m "Implement feature" && git push origin feature/YourFeature```
4. Ensure your work aligns with our [Coding Standards](https://github.com/2020wmarvil/glibby/blob/main/CodingStandard.md)
5. Create a pull request

Commit messages should be in the imperative mood, e.g. "Implement 2D curve intersection solver". 

## Contact

You can reach the project lead at 2020wmarvil@gmail.com, or join our [Discord server](https://discord.gg/jTcSZxsZ34).
