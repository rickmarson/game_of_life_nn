#include <torch/torch.h>
#include <torch/script.h>

using Module = torch::jit::script::Module; 

namespace models::gol {
    class ModelExecutor {
        public:
            ModelExecutor() = default;
            virtual ~ModelExecutor() = default;
            bool load(const std::string& module_path);

        protected:
            Module m_module;
    };
}