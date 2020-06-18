cmake_minimum_required(VERSION 3.17)

project(mazes CXX)

# Some basics, set accordingly to your needs.
set(CMAKE_CXX_STANDARD 17)

# Where the 'app' and 'blend2d' are.
set(PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Project directory")

set(LIB_DIR "${PROJECT_DIR}/lib"
    CACHE PATH "Project directory")
    
set(TEST_DIR "${PROJECT_DIR}/test"
    CACHE PATH "Project directory")

set(BLEND2D_DIR "${PROJECT_DIR}/../blend2d"
    CACHE PATH "Location of 'blend2d'")

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

# Your application target.
file(GLOB LIB_SRC_FILES ${LIB_DIR}/*.cpp)
file(GLOB LIB_HDR_FILES ${LIB_DIR}/*.hpp)
file(GLOB PY_BINDINGS_SRC_FILES ${LIB_DIR}/py/*.cpp)

add_library( mazes STATIC
             ${LIB_HDR_FILES}
             ${LIB_SRC_FILES} )

find_package(pybind11 REQUIRED)
pybind11_add_module(mazes_py ${PY_BINDINGS_SRC_FILES})

file(GLOB TEST_SRC_FILES ${TEST_DIR}/*.cpp)

add_executable(test_mazes ${TEST_SRC_FILES})

# Add Blend2D dependency to your application. The dependency
# should provide everything needed - include paths, libraries,
# compiler flags, and compiler definitions.
target_link_libraries(mazes Blend2D::Blend2D)
target_link_libraries(mazes_py PRIVATE mazes Blend2D::Blend2D)
target_link_libraries(test_mazes mazes Blend2D::Blend2D)
