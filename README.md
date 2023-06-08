# SPARK: Simulation and Physics Animation Rendering Kernel

![](https://i.imgur.com/IgNGQ3o.png)

SPARK is a general purpose game engine written in C++.

It is designed to be used during my 3 yers of studies at Ynov Bordeaux.

## Features
The engine is currently capable of rendering a simple 2D scene and can be used to create simple games like pong.

This engine will evolve during the next years. The following features are planned:
- 2D and 3D rendering (using Vulkan)
- Physics simulation
- Audio
- Editor

## Testing the engine
You have the following examples to try the engine:
- `pong` (Windows/Linux) A simple pong game

## Contributing

Feel free to contribute to this project by forking it and submitting a pull request. You can also open an issue if you find a bug or have a suggestion.

### Building the engine
To build the engine, you will need to install the following dependencies:
- [CMake](https://cmake.org/download/) (>= 3.25)
- [VCPKG](https://vcpkg.io/en/getting-started.html) with executable in PATH
- A C++ compiler (MSVC (>=19.31), GCC (>=13.1)) or Clang (>=16.0.0)

You have the following CMake presets to build the engine:

Configuration presets:
- `dev_vc143`, (Windows) using Visual Studio 2022 generator and MSVC compiler
- `dev_vc143_ninja` (Windows) using Ninja generator and MSVC compiler
- `dev_clang_ninja` (Windows & Linux) using Ninja generator and Clang compiler
- `dev_gcc_ninja` (Linux) using Ninja generator and GCC compiler

Build presets:
- `dev_vc143_debug` (Windows) Debug VS MSVC build
- `dev_vc143_release` (Windows) Release VS MSVC build
- `dev_vc143_ninja_debug` (Windows) Debug Ninja MSVC build
- `dev_vc143_ninja_release` (Windows) Release Ninja MSVC build
- `dev_clang_ninja_debug` (Windows & Linux) Debug Ninja Clang build7
- `dev_clang_ninja_release` (Windows & Linux) Release Ninja Clang build
- `dev_gcc_ninja_debug` (Linux) Debug Ninja GCC build
- `dev_gcc_ninja_release` (Linux) Release Ninja GCC build

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
