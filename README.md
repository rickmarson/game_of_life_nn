A Pytorch implementation of Conway's Game of Life

A very simple CNN that learns the rules of the classic [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) and can then evolve any initial grid configuration. 

## Highlights

- Demonstrates training, running and tracing a simple Pytorch NN
- Interoperability with C++ / LibTorch. The `runner` project demonstrates how to run a model trained in Python within a C++ "game".
- Using a C++ library within Python to accelerate the generation of grid samples for training. 

## System Requirements

- An NVidia GPU
- Python 3
- `pip3 install pygame`
- [CMake](https://cmake.org/) 3.16 or higher
- [Pytorch](https://pytorch.org/)
- [LibTorch](https://pytorch.org/)
- [CUDA](https://developer.nvidia.com/cuda-toolkit)
- [CuDNN](https://developer.nvidia.com/cudnn)

Tested with:
- Ubuntu 20.04, under WSL2 on Windows11
- gcc 9.3.0
- Python 3.9
- Torch 1.10
- Cuda 11.5
- Visual Studio Code
- CMake configs are provided for Linux only

### List of third party libraries

- [Blend2D](https://blend2d.com/)
- [asmjit](https://github.com/asmjit/asmjit). Required by Blend2D
- [pybind11](https://github.com/pybind/pybind11)
- [SDL2](https://www.libsdl.org/)

- Run `git submodule update --init --recursive` to initialise thirdparty dependencies (they will all be placed in `./utilities`)
