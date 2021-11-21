#include "model_executor.hpp"

namespace models::gol {

    bool ModelExecutor::load(const std::string& module_path) {
        if (!torch::cuda::is_available()) {
            std::cerr << "CUDA not available! Failed to load model" << std::endl;
            return false;
        }

        try {
            m_module = torch::jit::load(module_path.c_str(), torch::kCUDA);
        }
        catch (const c10::Error& e) {
            std::cerr << "error loading the model\n" << e.what() << std::endl;
            return false;
        }
        return true;
    }
}