#include "game_of_life_runner_grid.hpp"
#include <SDL.h>
#include <filesystem>

namespace models::gol {

    void GameOfLifeGridRunner::begin_run() {
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);

        m_gol.initialise(m_grid_width, m_grid_height, m_width / m_grid_width);
        m_current_grid = m_gol.get_current_grid();
        auto model_path = std::string("models/saved/game_of_life_grid.torchscript");
        m_inference.load(model_path);
    }

    void GameOfLifeGridRunner::update() {
        m_current_grid = m_inference.forward(m_current_grid);
        auto grid_pixels = m_gol.render_grid(m_current_grid, false, false);
        render_rgb_image(grid_pixels);
    }

    void GameOfLifeGridRunner::end_run() {
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
        }
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }
    }

    void GameOfLifeGridRunner::render_rgb_image(const std::vector<uint8_t>& pixels) {
        int pitch;
        void* dst_pixels = nullptr;
        SDL_LockTexture(m_texture, NULL, &dst_pixels, &pitch);
        SDL_memcpy(dst_pixels, pixels.data(), pitch * m_height);
        SDL_UnlockTexture(m_texture);
        SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);        
    }
}