cmake_minimum_required(VERSION 3.16)

project(game_of_life_nn CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-fPIC)

set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(BLEND2D_DIR "${PROJECT_DIR}/thirdparty/blend2d"
    CACHE PATH "Location of 'blend2d'")

set(SDL_DIR "${PROJECT_DIR}/thirdparty/SDL"
    CACHE PATH "Location of 'SDL'")

set(PYBIND_DIR "${PROJECT_DIR}/thirdparty/pybind11"
    CACHE PATH "Location of 'pybind11'")

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

set(PYBIND11_PYTHON_VERSION "3.9")
set(PYBIND11_FINDPYTHON ON)
add_subdirectory("${PYBIND_DIR}")
add_subdirectory("${SDL_DIR}")

set(CUDA_HOME "/usr/local/cuda")
set(CUDA_TOOLKIT_ROOT_DIR ${CUDA_HOME})
set(CUDA_NVCC_EXECUTABLE "${CUDA_HOME}/bin/nvcc")

set(torch_DIR "/usr/lib/libtorch/share/cmake/Torch")
find_package(torch REQUIRED)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/game_of_life")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/runner")
