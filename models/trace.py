import os
import torch

from prediction import GridPredictionModel


def load_saved_model():
    model_path = os.path.join(os.path.dirname(__file__), 'saved/game_of_life_grid.pt')
    model = GridPredictionModel()
    model.load_state_dict(torch.load(model_path))
    model.eval()
    return model


def trace_model(model, device):
    output_folder = os.path.join(os.path.dirname(__file__), 'saved')
    trace_file_path = os.path.join(output_folder, 'game_of_life_grid.torchscript')
    random_input = torch.rand(1, 1, 50, 50, dtype=torch.float32).to(device)
    traced_script_module = torch.jit.trace(model, random_input)
    traced_script_module.save(trace_file_path)


def main():
    if torch.cuda.is_available():
        device = torch.device("cuda")
    else:
        raise ValueError("No CUDA device found!")

    with torch.no_grad():
        model = load_saved_model()
        model.to(device)
        trace_model(model, device)


if __name__ == "__main__":
    main()
