#include "grid_model_executor.hpp"

namespace models::gol {

    at::Tensor to_float_tensor(const std::vector<std::vector<bool>>& grid) {
        auto options = torch::TensorOptions()
                       .dtype(torch::kFloat32)
                       .device(torch::kCUDA);
        auto tensor = torch::zeros({50, 50}, options);

        for (auto y = 0; y < grid.size(); y++) {
            for (auto x = 0; x < grid[y].size(); x++) {
                if (grid[y][x]) {
                    tensor.index_put_({y, x}, 1.0f);
                }
            }
        }

        return tensor;
    }

    std::vector<std::vector<float>> to_std_vector(const at::Tensor& tensor) {
        auto shape = tensor.sizes();
        std::vector<std::vector<float>> out;

        if (shape.size() != 2 || shape[0] != 50 || shape[1] != 50) {
            std::cerr << "Unexpected output shape (" << shape[0] << " , " << shape[1] << ")" << std::endl;
            return out;
        }

        out.resize(shape[0]);
        for (auto y = 0; y < shape[0]; y++) {
            out[y].resize(shape[1]);
            for (auto x = 0; x < shape[1]; x++) {
                out[y][x] = tensor.index({y, x}).item<float>();
            }
        }

        return std::move(out);
    }

    std::vector<std::vector<bool>> to_bool_grid(const std::vector<std::vector<float>>& float_grid) {
        std::vector<std::vector<bool>> out;
        out.resize(float_grid.size());
        for (auto y = 0; y < float_grid.size(); y++) {
            out[y].resize(float_grid[y].size());
            for (auto x = 0; x < float_grid[y].size(); x++) {
                if (float_grid[y][x] > 0.5f) {
                    out[y][x] = true;
                } else {
                    out[y][x] = false;
                }
            }
        }
        return std::move(out);
    }

    std::vector<std::vector<bool>> GridModelExecutor::forward(const std::vector<std::vector<bool>>& grid_in) {
        try {
            auto tensor_in = to_float_tensor(grid_in);
            tensor_in.unsqueeze_(0)
                     .unsqueeze_(1);

            std::vector<torch::jit::IValue> inputs;
            inputs.push_back(tensor_in);
        
            auto output = m_module.forward(inputs)
                                  .toTensor()
                                  .cpu();
            
            output.squeeze_();

            m_predicted_grid = to_std_vector(output);

            return std::move(to_bool_grid(m_predicted_grid));
        } catch (const std::exception& e) {
            std::cerr << "error during model forward pass: " << e.what() << std::endl;
            throw(e);
        }
    }
}