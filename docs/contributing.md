# Contributing

Feel free to contribute to this project by forking it and submitting a pull request. You can also open an issue if you find a bug or have a suggestion.

Please follow the templates whe opening an issue or submitting a pull request.
For code contributions, please follow the [style guidelines](style-guidelines.md).

## Requirements
To build the engine, you will need to install the following dependencies:
- ![](docs/img/cmake.png) [CMake](https://cmake.org/download/) (>= 3.25)
- ![](docs/img/vcpkg.png) [VCPKG](https://vcpkg.io/en/getting-started.html) with executable in PATH
- A C++ 20 compiler:
    - ![](docs/img/msvc.png) MSVC (>=19.31)
    - ![](docs/img/gcc.png) GCC (>=13.1)
    - ![](docs/img/clang.png) Clang (>=16.0.0)

## Building & Testing the engine
You have the following CMake presets to build the engine:

<u>Configuration presets:</u>
- `dev_vc143`, (Windows) using Visual Studio 2022 generator and MSVC compiler
- `dev_vc143_ninja` (Windows) using Ninja generator and MSVC compiler
- `dev_clang_ninja` (Windows & Linux) using Ninja generator and Clang compiler
- `dev_gcc_ninja` (Linux) using Ninja generator and GCC compiler

To use them, you can use the following command:
```bash
cmake --preset <preset_name>
```
or use your IDE CMake integration and select the preset you want.

<u>Build presets:</u>  
A build preset is composed of a configuration preset and a build type (`debug` or `release`) .  
For example, `dev_vc143_debug` is a build configured from `dev_vc143` in debug mode.

To build the engine, you can use the following command:
```bash
cmake --build --preset <build_preset_name>
```
or use your IDE CMake integration and build with the configuration you want.

<u>Test presets:</u>  
Tests preset have the same name as build presets, but there are launched with CTest.

To run the tests, you can use the following command:
```bash
ctest --preset <test_preset_name>
```
, use the test preset in your IDE CMake integration or run the test executables directly.
