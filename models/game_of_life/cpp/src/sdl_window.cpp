#include "sdl_window.hpp"
#include <SDL.h>

namespace models::gol {
    void SdlWindow::init(int w, int h) {
        m_width = w;
        m_height = h;

        SDL_Init(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
    }

    void SdlWindow::run() {
        SDL_Event event;
        m_stop = false;

        begin_run();

        auto time = SDL_GetTicks();
        while (!m_stop)
        {
            SDL_WaitEventTimeout(&event, 10);
    
            switch (event.type)
            {
            case SDL_QUIT:
                m_stop = true;
                break;
            }
            
            if (SDL_GetTicks() - time >= 40) {
                update();
                time = SDL_GetTicks();
            }
        }

        end_run();

        if (m_window) {
            SDL_DestroyWindow(m_window);
        }

        SDL_Quit();
    }

    void SdlWindow::stop() {
        m_stop = true;
    }

}