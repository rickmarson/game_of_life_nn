#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../game_of_life.hpp"

PYBIND11_MODULE(game_of_life_py, m) {
    m.doc() = "game of life python bindings"; 

    pybind11::class_<common::gol::GameOfLife>(m, "GameOfLife")
        .def(pybind11::init<>())
        .def_property_readonly("step", &common::gol::GameOfLife::get_current_step)
        .def("initialise", &common::gol::GameOfLife::initialise)
        .def("set_output_dir", &common::gol::GameOfLife::set_output_dir)
        .def("update", &common::gol::GameOfLife::update)
        .def("reset", &common::gol::GameOfLife::reset)
        .def("get_current_grid", &common::gol::GameOfLife::get_current_grid)
        .def("get_last_grid", &common::gol::GameOfLife::get_last_grid)
        .def("render_current_grid", &common::gol::GameOfLife::render_current_grid)
        .def("render_last_grid", &common::gol::GameOfLife::render_last_grid)
        .def("render_current_grid_to_file", &common::gol::GameOfLife::render_current_grid_to_file)
        .def("render_last_grid_to_file", &common::gol::GameOfLife::render_last_grid_to_file)
        .def("render_grid", &common::gol::GameOfLife::render_grid)
        .def("render_probabilities", &common::gol::GameOfLife::render_probabilities);
}
