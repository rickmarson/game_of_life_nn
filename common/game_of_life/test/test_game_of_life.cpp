#include "../lib/game_of_life.hpp"

int main(int argc, char* argv[]) {
    using namespace common::gol;
    auto gol = GameOfLife();
    gol.initialise(20, 20, 24);
    gol.update();
    gol.render_current_grid_to_file();
    gol.render_last_grid_to_file();
}