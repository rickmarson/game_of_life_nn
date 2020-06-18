#include "game_of_life.hpp"
#include <blend2d.h>
#include <random>
#include <functional>
#include <algorithm>

namespace common::gol {

void GameOfLife::initialise(int grid_width, int grid_height, int cell_pixel_size) {
  m_width = grid_width;
  m_height = grid_height;
  m_cell_pixel_size = cell_pixel_size;

  std::random_device rng;
  std::mt19937 urng(rng());

  m_current_grid.resize(m_height);
  for (auto& row : m_current_grid) { 
    row.resize(m_width, false);
    for (auto i = 0; i < m_width / 2; i++) {
      row[i] = true;
    }

    std::shuffle(row.begin(), row.end(), urng);
  }
  
  m_last_grid = m_current_grid;
} 

void GameOfLife::update() {
  Grid temp;
  temp.resize(m_height);
  for (auto y = 0; y < m_height; y++) {
    temp[y].resize(m_width);
    for (auto x = 0; x < m_width; x++) {
      auto alive_count = 0;
      for (const auto& loc : neighbour_offsets) {
        auto n_y = y + loc.y;
        auto n_x = x + loc.x;
        if ( n_y < 0 || n_y >= m_height || n_x < 0 || n_x >= m_width) {
          continue;
        }
        if (m_current_grid[n_y][n_x]) {
          ++alive_count;
        }
      }
      if (m_current_grid[y][x] && (alive_count < 2 || alive_count > 3)) {
        temp[y][x] = false;
      } else if (!m_current_grid[y][x] && alive_count == 3) {
        temp[y][x] = true;
      } else {
        temp[y][x] = m_current_grid[y][x];
      }
    }
  }

  m_last_grid = std::move(m_current_grid);
  m_current_grid = std::move(temp);
  
  ++m_step;
}

void GameOfLife::reset() {
  std::random_device rng;
  std::mt19937 urng(rng());

  m_current_grid.resize(m_height);
  for (auto& row : m_current_grid) { 
    row.resize(m_width, false);
    for (auto i = 0; i < m_width / 2; i++) {
      row[i] = true;
    }

    std::shuffle(row.begin(), row.end(), urng);
  }
  
  m_last_grid = m_current_grid;
  m_step = 0;
}

std::vector<uint8_t> GameOfLife::render_current_grid() {
  std::vector<uint8_t> out;
  render_grid_internal(m_current_grid, false, out);
  return std::move(out);
}

std::vector<uint8_t> GameOfLife::render_last_grid() {
  std::vector<uint8_t> out;
  render_grid_internal(m_last_grid, false, out);
  return std::move(out);
}
    
void GameOfLife::render_current_grid_to_file() {
  std::vector<uint8_t> dummy;
  render_grid_internal(m_current_grid, true, dummy, "current");
}

void GameOfLife::render_last_grid_to_file() {
  std::vector<uint8_t> dummy;
  render_grid_internal(m_last_grid, true, dummy, "last");
}

std::vector<uint8_t> GameOfLife::render_grid(const Grid& grid, bool save_to_file, bool grayscale) {
  std::vector<uint8_t> out;
  render_grid_internal(grid, save_to_file, out, "extern", grayscale);
  return std::move(out);
}

void GameOfLife::render_grid_internal(const Grid& grid, bool save_to_file, std::vector<uint8_t>& pixels_out,  std::string file_name, bool grayscale) {
  const auto w = m_width * m_cell_pixel_size;
  const auto h = m_height * m_cell_pixel_size;
  
  BLImage img(w, h, BL_FORMAT_XRGB32);

  // Attach a rendering context into `img`.
  BLContext ctx(img);

  // Clear the image.
  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
  ctx.fillAll();

  draw_grid_lines(ctx);
  fill_grid(ctx, grid);
  
  // Detach the rendering context from `img`.
  ctx.end();

  if (!save_to_file) {
    BLImageData pixels;
    auto out = std::vector<uint8_t>();
    // convert to 1-byte grayscale
    auto out_size = grayscale ? w * h : w * h *4;
    if (img.getData(&pixels) == BL_SUCCESS) 
    {
      pixels_out.resize(out_size);
      auto dst = pixels_out.data();
      const auto src = static_cast<const uint8_t*>(pixels.pixelData);
      if (grayscale) {
        for (auto i = 0, j = 0; i < pixels.stride * h; i = i + 4, j++) {
          dst[j] = (src[i] + src[i + 1] + src[i + 2]) / 3;
        }
      } else {
        std::memcpy(dst, src, out_size);
      }
    }
  } else {
    save_image_to_file(img);
  }
}

std::vector<std::vector<uint8_t>> GameOfLife::render_probabilities(const ProbabilisticGrid& grid, bool save_to_file) {
  const auto w = m_width * m_cell_pixel_size;
  const auto h = m_height * m_cell_pixel_size;
  std::vector<std::vector<uint8_t>> pixels_out;

  BLImage img(w, h, BL_FORMAT_XRGB32);

  // Attach a rendering context into `img`.
  BLContext ctx(img);

  // Clear the image.
  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
  ctx.fillAll();

  draw_grid_lines(ctx);
  fill_grid_shaded(ctx, grid);
  
  // Detach the rendering context from `img`.
  ctx.end();

  if (!save_to_file) {
    BLImageData pixels;
    // convert to 1-byte grayscale
    auto out_size = w * h;
    if (img.getData(&pixels) == BL_SUCCESS) 
    {
      pixels_out.resize(out_size);
      const auto src = static_cast<const uint8_t*>(pixels.pixelData);
      for (auto i = 0, j = 0; i < pixels.stride * h; i = i + 4, j++) {
        pixels_out[j].resize(3);
        pixels_out[j][0] = src[i];
        pixels_out[j][1] = src[i + 1];
        pixels_out[j][2] = src[i + 2];
      }
    }

    if (save_to_file) {
      save_image_to_file(img);
    }
  }

  return std::move(pixels_out);
}


void GameOfLife::draw_grid_lines(BLContext& ctx) {
  const auto w = m_width * m_cell_pixel_size;
  const auto h = m_height * m_cell_pixel_size;

  auto line_colour = BLRgba32(0xFF808080);
  // horizontal
  for (auto i = 0; i < h; i++) {
    BLPath path;

    path.moveTo(0, i*m_cell_pixel_size);   
    path.lineTo(w, i*m_cell_pixel_size);
    ctx.setCompOp(BL_COMP_OP_SRC_OVER);
    ctx.setStrokeStyle(line_colour);
    ctx.setStrokeWidth(2);
    ctx.setStrokeStartCap(BL_STROKE_CAP_ROUND);
    ctx.setStrokeEndCap(BL_STROKE_CAP_BUTT);
    ctx.strokePath(path);
  }

  // vertical
  for (auto i = 0; i < w; i++) {
    BLPath path;

    path.moveTo(i*m_cell_pixel_size, 0);   
    path.lineTo(i*m_cell_pixel_size, h);
    ctx.setCompOp(BL_COMP_OP_SRC_OVER);
    ctx.setStrokeStyle(line_colour);
    ctx.setStrokeWidth(2);
    ctx.setStrokeStartCap(BL_STROKE_CAP_ROUND);
    ctx.setStrokeEndCap(BL_STROKE_CAP_BUTT);
    ctx.strokePath(path);
  }
}

void GameOfLife::fill_grid(BLContext& ctx, const Grid& grid) {
  // fill in active cells
  auto fill_colour = BLRgba32(0xFF000000);
  ctx.setCompOp(BL_COMP_OP_SRC_OVER);
  ctx.setFillStyle(fill_colour);

  for (auto y = 0; y < m_height; y++) {
    for (auto x = 0; x < m_width; x++) {
      if (grid[y][x]) {
        ctx.fillRect(x * m_cell_pixel_size, y * m_cell_pixel_size, m_cell_pixel_size, m_cell_pixel_size);
      }
    }
  }
}

void GameOfLife::fill_grid_shaded(BLContext& ctx, const ProbabilisticGrid& grid) {  
  // fill in active cells
  ctx.setCompOp(BL_COMP_OP_SRC_OVER);
  
  for (auto y = 0; y < m_height; y++) {
    for (auto x = 0; x < m_width; x++) {
      auto fill_colour = BLRgba32(45, grid[y][x]*255, 45);
      ctx.setFillStyle(fill_colour);
      ctx.fillRect(x * m_cell_pixel_size, y * m_cell_pixel_size, m_cell_pixel_size, m_cell_pixel_size);
    }
  }
}

void GameOfLife::save_image_to_file(BLImage& img) {
  BLImageCodec codec;
  codec.findByName("BMP");
  auto file_name = std::string{};
  if (!m_output_dir.empty()) {
    file_name += m_output_dir;
  }
  file_name += file_name + "_";
  file_name += std::to_string(m_step) + ".bmp";
  img.writeToFile(file_name.c_str(), codec);
}

}
