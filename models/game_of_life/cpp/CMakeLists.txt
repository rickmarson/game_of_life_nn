cmake_minimum_required(VERSION 3.17)

project(game_of_life_runner CXX)

set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    CACHE PATH "Project directory")

# find libtorch
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../../common/libtorch")
find_package(torch REQUIRED)

# find SDL
set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../SDL2-2.0.12") 
set(SDL_INCLUDE_DIR "${SDL2_DIR}/include") 
set(SDL_LIBRARIES "${SDL2_DIR}/lib/x64/SDL2.lib" "${SDL2_DIR}/lib/x64/SDL2main.lib")
include_directories(${SDL_INCLUDE_DIR})

# compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# local dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../common/game_of_life" "${CMAKE_CURRENT_BINARY_DIR}/game_of_life")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../common/game_of_life/lib")

set(SRC_DIR "${PROJECT_DIR}/src"
    CACHE PATH "Project directory")

file(GLOB LIB_SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB LIB_HDR_FILES ${SRC_DIR}/*.hpp)

add_executable(game_of_life_runner ${LIB_HDR_FILES} ${LIB_SRC_FILES})
target_link_libraries(game_of_life_runner game_of_life "${TORCH_LIBRARIES}" "${SDL_LIBRARIES}")

if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET game_of_life_runner
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:game_of_life_runner>)
    file(GLOB SDL_DLLS "${SDL2_DIR}/lib/x64/*.dll")
    add_custom_command(TARGET game_of_life_runner
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${SDL_DLLS}
                     $<TARGET_FILE_DIR:game_of_life_runner>)
    add_custom_command(TARGET game_of_life_runner
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     "${CMAKE_CURRENT_SOURCE_DIR}/../models/game_of_life_grid.torchscript"
                     $<TARGET_FILE_DIR:game_of_life_runner>)
endif (MSVC)