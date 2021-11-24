
set(RUNNER_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    CACHE PATH "Project directory")

# compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set(SRC_DIR "${RUNNER_DIR}/src"
    CACHE PATH "Project directory")

file(GLOB LIB_SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB LIB_HDR_FILES ${SRC_DIR}/*.hpp)

include_directories("${SDL_DIR}/include")
include_directories("${PROJECT_DIR}/game_of_life/lib")

add_executable(game_of_life_runner ${LIB_HDR_FILES} ${LIB_SRC_FILES})
target_link_libraries(game_of_life_runner game_of_life SDL2main SDL2 "${TORCH_LIBRARIES}")
