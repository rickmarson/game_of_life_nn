import mazes_py as mz
import numpy as np
from PIL import Image


if __name__ == "__main__":
    w = 480
    h = 480
    pixel_data = mz.gen_test_image()
    pixels = np.asarray(pixel_data)
    pixels = pixels.reshape(h, w, 3).astype(np.uint8)
    image = Image.fromarray(pixels, 'RGB')
    image.save('test.png')
    image.show()
