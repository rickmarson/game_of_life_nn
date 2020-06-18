#include "game_of_life_runner_grid.hpp"

using namespace models::gol;

int main(int& argc, char** argv) {
    auto runner = GameOfLifeGridRunner();
    runner.init(1200, 1200);
    runner.run();
    return 0;
}
