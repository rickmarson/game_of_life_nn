#include "mazes.hpp"
#include <blend2d.h>
#include <cstring>

std::vector<uint8_t> gen_test_image() 
{
  auto w = 480;
  auto h = 480;
  
  BLImage img(w, h, BL_FORMAT_XRGB32);

  // Attach a rendering context into `img`.
  BLContext ctx(img);

  // Clear the image.
  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.fillAll();

  // Fill some path.
  BLPath path;
  path.moveTo(26, 31);
  path.cubicTo(642, 132, 587, -136, 25, 464);
  path.cubicTo(882, 404, 144, 267, 27, 31);

  ctx.setCompOp(BL_COMP_OP_SRC_OVER);
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
  ctx.fillPath(path);

  // Detach the rendering context from `img`.
  ctx.end();

  BLImageData pixels;
  auto out = std::vector<uint8_t>();
  auto out_stride = w * 3;
  auto out_size = out_stride * h;
  if (img.getData(&pixels) == BL_SUCCESS) 
  {
    out.resize(out_size);
    auto dst = out.data();
    const auto src = static_cast<const uint8_t*>(pixels.pixelData);
    for (auto i = 0, j = 0; i < pixels.stride * h; i = i + 4, j = j + 3) {
      dst[j] = src[i];
      dst[j + 1] = src[i + 1];
      dst[j + 2] = src[i + 2];
    }
  }
  
  /*BLImageCodec codec;
  codec.findByName("BMP");
  img.writeToFile("bl-getting-started-1.bmp", codec);*/

  return std::move(out);
}
