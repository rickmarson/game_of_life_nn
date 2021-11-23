#include <vector>
#include <stdint.h>

class SDL_Window;

namespace models::gol {
    class SdlWindow {
        public:
            SdlWindow() = default;
            virtual ~SdlWindow() = default;
            void init(int w, int h);
            void run();
            void stop();

        protected:
            SDL_Window* m_window = nullptr;
            bool m_stop = true;
            uint32_t m_width = 0;
            uint32_t m_height = 0;

            virtual void begin_run() = 0;
            virtual void update() = 0;
            virtual void end_run() = 0;
    };
}