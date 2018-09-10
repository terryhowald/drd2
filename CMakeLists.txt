project (drd2)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${drd2_SOURCE_DIR}/sdl2-cmake-scripts")

set(SOURCE main.cpp game.cpp summary.cpp sprite.cpp spritegroup.cpp 
    player.cpp enemy.cpp tile.cpp egg.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} 
    ${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES} 
    ${SDL2TTF_LIBRARIES})

