#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../mazes.hpp"

PYBIND11_MODULE(mazes_py, m) 
{
    m.doc() = "mazes python bindings"; 

    m.def("gen_test_image", &gen_test_image, "Test");
}
