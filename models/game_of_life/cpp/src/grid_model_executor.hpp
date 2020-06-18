#include "model_executor.hpp"

namespace models::gol {
    class GridModelExecutor : public ModelExecutor {
        public:
            GridModelExecutor() = default;
            ~GridModelExecutor() = default;

            std::vector<std::vector<bool>> forward(const std::vector<std::vector<bool>>& grid_in);
            const std::vector<std::vector<float>>& get_predicted_grid() const { return m_predicted_grid; }

        private:
            std::vector<std::vector<float>> m_predicted_grid;
    };
}