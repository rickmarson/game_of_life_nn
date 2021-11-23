import numpy as np
import os
import torch
import pygame
import time

from game_of_life_py import GameOfLife
from prediction import GridPredictionModel


def load_model(device):
    model_path = os.path.join(os.path.dirname(__file__), 'saved/game_of_life_grid.pt')
    model = GridPredictionModel()
    model.load_state_dict(torch.load(model_path))
    model.eval()
    for param in model.parameters():
        param.requires_grad_(False)
    model.to(device)
    return model


def convert_model_output(predicted_grid):
    predicted_grid_bools = np.zeros(predicted_grid.shape, dtype=bool)
    for i, row in enumerate(predicted_grid):
        for j, cell in enumerate(row):
            if cell > 0.5:
                predicted_grid_bools[i][j] = True
    return predicted_grid_bools


def render_grid(pixel_data, screen, pixel_w, pixel_h):
    pixel_grid = np.asarray(pixel_data).astype(np.uint8)
    pixel_grid = pixel_grid.reshape(pixel_h, pixel_w)

    screenarray = np.zeros((pixel_h, pixel_w, 3), dtype=np.uint8)
    screenarray[:, :, 2] = screenarray[:, :, 1] = screenarray[:, :, 0] = pixel_grid

    pygame.surfarray.blit_array(screen, screenarray)


def main():
    if torch.cuda.is_available():
        device = torch.device("cuda")
    else:
        raise ValueError("No CUDA device found!")

    w = 50
    h = 50
    cell_pixel_size = 24
    iterations = 100

    pygame.init()
    screen = pygame.display.set_mode((w*cell_pixel_size, h*cell_pixel_size), pygame.DOUBLEBUF)
    pygame.display.set_caption('GameOfLife Run Model')

    gol = GameOfLife()
    gol.initialise(w, h, cell_pixel_size)
    grid_np = np.asarray(gol.get_current_grid()).astype(np.float32)
    grid = torch.from_numpy(grid_np).to(device)
    grid.unsqueeze_(0)
    grid.unsqueeze_(1)

    model = load_model(device)

    for i in range(iterations):
        # gol.update()
        new_grid = model.forward(grid)
        grid = new_grid
        new_grid_np = new_grid.squeeze().cpu().detach().numpy()
        
        new_grid_np = convert_model_output(new_grid_np)
        pixel_data = gol.render_grid(new_grid_np.tolist(), False)
        render_grid(pixel_data, screen, w * cell_pixel_size, h * cell_pixel_size)
        
        pygame.display.flip()

        time.sleep(0.04)

    pygame.quit()


if __name__ == "__main__":
    main()
