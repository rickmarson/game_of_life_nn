#include <vector>
#include <stdint.h>
#include <string>

class BLContext;
class BLImage;

namespace common {
namespace gol {

typedef std::vector<std::vector<bool>> Grid;
typedef std::vector<std::vector<float>> ProbabilisticGrid;

class GameOfLife {
  public:
    GameOfLife() = default;
    ~GameOfLife() = default;
    
    void initialise(int grid_width, int grid_height, int pixel_size);
    void set_output_dir(const std::string& dir) { m_output_dir = dir; }
    void update();
    void reset();
    int get_current_step() const { return m_step; }
    const Grid& get_current_grid() const { return m_current_grid; }
    const Grid& get_last_grid() const { return m_last_grid; }
    [[nodiscard]] std::vector<uint8_t> render_current_grid();
    [[nodiscard]] std::vector<uint8_t> render_last_grid();
    void render_current_grid_to_file();
    void render_last_grid_to_file();

    std::vector<uint8_t> render_grid(const Grid& grid, bool save_to_file = false, bool grayscale = true);
    std::vector<std::vector<uint8_t>> render_probabilities(const ProbabilisticGrid& grid, bool save_to_file = false);

  private:
    void render_grid_internal(const Grid& grid, bool save_to_file, std::vector<uint8_t>& pixels_out, std::string file_name = "",  bool grayscale = true);
    void draw_grid_lines(BLContext& ctx);
    void fill_grid(BLContext& ctx, const Grid& grid);
    void fill_grid_shaded(BLContext& ctx, const ProbabilisticGrid& grid);
    void save_image_to_file(BLImage& img);

    int m_step = 0;
    int m_width = 0;
    int m_height = 0;
    int m_cell_pixel_size = 0;
    std::string m_output_dir;
    Grid m_current_grid;
    Grid m_last_grid;

    struct Location {
        int x;
        int y;
    };
    const std::vector<Location> neighbour_offsets = { {-1, -1},
                                                     {-1, 0},
                                                     {-1, 1},
                                                     {0, -1},
                                                     {0, 1},
                                                     {1, -1},
                                                     {1, 0},
                                                     {1, 1} };
};

}
}
