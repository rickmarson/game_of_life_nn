#include "sdl_window.hpp"
#include "grid_model_executor.hpp"
#include "game_of_life.hpp"

class SDL_Renderer;
class SDL_Texture;

namespace models::gol {
    class GameOfLifeGridRunner : public SdlWindow {
        public:
            GameOfLifeGridRunner() = default;
            virtual ~GameOfLifeGridRunner() = default;

        private:
            SDL_Renderer* m_renderer = nullptr;
            SDL_Texture* m_texture = nullptr;

            const uint32_t m_grid_width = 50;
            const uint32_t m_grid_height = 50;
            common::gol::Grid m_current_grid;
            common::gol::GameOfLife m_gol;
            GridModelExecutor m_inference;
            
            void begin_run() final;
            void update() final;
            void end_run() final;

            void render_rgb_image(const std::vector<uint8_t>& pixels);
    };
}