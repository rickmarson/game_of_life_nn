from game_of_life_py import GameOfLife
import numpy as np
import os
import torch
import torch.nn as nn
from PIL import Image

from prediction import GridPredictionModel
from trace import trace_model


def get_cuda_device():
    if torch.cuda.is_available():
        device = torch.device("cuda")
    else:
        raise ValueError("No CUDA device found!")

    return device


def show_grid(w, h, pixel_size, pixel_data):
    pixels = np.asarray(pixel_data).astype(np.uint8)
    pixels = pixels.reshape(h*pixel_size, w*pixel_size)
    image = Image.fromarray(pixels, 'L')
    image.show()


def show_probabilities(w, h, pixel_size, pixel_data):
    pixels = np.asarray(pixel_data).astype(np.uint8)
    pixels = pixels.reshape(h*pixel_size, w*pixel_size, 3)
    image = Image.fromarray(pixels, 'RGB')
    image.show()


def save_model(model):
    output_folder = os.path.join(os.path.dirname(__file__), 'models')
    # for python
    torch.save(model.state_dict(), os.path.join(output_folder, 'game_of_life_grid.pt'))
    # for c++
    with torch.no_grad():
        trace_model(model, get_cuda_device())


def train_grid_model(w, h, cell_pixel_size, device):
    gol = GameOfLife()
    gol.initialise(w, h, cell_pixel_size)

    model = GridPredictionModel()
    model.to(device)

    loss = nn.MSELoss()
    optim = torch.optim.Adam(model.parameters(), lr=1e-4)

    episodes = 200
    steps = 100

    for episode in range(episodes):
        for step in range(steps):
            gol.update()

            current_grid_src = gol.get_current_grid()
            last_grid_src = gol.get_last_grid()
            current_grid_array = np.asarray(current_grid_src).astype(np.float32)
            last_grid_array = np.asarray(last_grid_src).astype(np.float32)

            real_grid = torch.from_numpy(current_grid_array).to(device)

            tensor_in = torch.from_numpy(last_grid_array).to(device)
            
            # network input is a 4d tensor: [batch, channels, h, w]
            tensor_in.unsqueeze_(0)
            tensor_in.unsqueeze_(1)
            real_grid.unsqueeze_(0)
            real_grid.unsqueeze_(1)

            tensor_out = model.forward(tensor_in)

            optim.zero_grad()
            grid_loss = loss(tensor_out, real_grid)
            grid_loss.backward()
            optim.step()

            if (step+1) % 100 == 0:
                print('Episode {}/{}, Step {}/{}, loss = {:.4f}'.format(episode+1, episodes, step+1, steps, grid_loss))
        
        gol.reset()

    predicted_grid = tensor_out.squeeze().cpu().detach().numpy()
    show_probabilities(w, h, cell_pixel_size, gol.render_probabilities(predicted_grid.tolist(), False))

    predicted_grid_bools = np.zeros(predicted_grid.shape, dtype=bool)
    for i, row in enumerate(predicted_grid):
        for j, cell in enumerate(row):
            if cell > 0.5:
                predicted_grid_bools[i][j] = True

    show_grid(w, h, cell_pixel_size, gol.render_grid(predicted_grid_bools.tolist(), False))

    save_model(model)


def main():
    device = get_cuda_device()
    
    w = 50
    h = 50
    cell_pixel_size = 24

    train_grid_model(w, h, cell_pixel_size, device)
    

if __name__ == "__main__":
   main()

    
