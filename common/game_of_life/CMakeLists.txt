cmake_minimum_required(VERSION 3.17)

project(game_of_life CXX)

# Some basics, set accordingly to your needs.
set(CMAKE_CXX_STANDARD 17)

# Where the 'app' and 'blend2d' are.
set(GOL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    CACHE PATH "Project directory")

set(LIB_DIR "${GOL_LIB_DIR}/lib"
    CACHE PATH "Project directory")
    
set(TEST_DIR "${GOL_LIB_DIR}/test"
    CACHE PATH "Project directory")

set(BLEND2D_DIR "${GOL_LIB_DIR}/../blend2d"
    CACHE PATH "Location of 'blend2d'")

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

# Your application target.
file(GLOB LIB_SRC_FILES ${LIB_DIR}/*.cpp)
file(GLOB LIB_HDR_FILES ${LIB_DIR}/*.hpp)
file(GLOB PY_BINDINGS_SRC_FILES ${LIB_DIR}/py/*.cpp)

add_library( game_of_life STATIC
             ${LIB_HDR_FILES}
             ${LIB_SRC_FILES} )

find_package(pybind11 REQUIRED)
pybind11_add_module(game_of_life_py ${PY_BINDINGS_SRC_FILES})

file(GLOB TEST_SRC_FILES ${TEST_DIR}/*.cpp)

add_executable(test_game_of_life ${TEST_SRC_FILES})

# Add Blend2D dependency to your application. The dependency
# should provide everything needed - include paths, libraries,
# compiler flags, and compiler definitions.
target_link_libraries(game_of_life Blend2D::Blend2D)
target_link_libraries(game_of_life_py PRIVATE game_of_life Blend2D::Blend2D)
target_link_libraries(test_game_of_life game_of_life Blend2D::Blend2D)
