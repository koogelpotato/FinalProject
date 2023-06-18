#pragma once

#include <vector>

#include "game-object.hpp"
#include "resource-manager.hpp"
#include "sprite-renderer.hpp"

class Game_Level
{
public:
    std::vector<Game_Object> bricks;

    Game_Level() {}

    void load(const char*  file,
              unsigned int level_width,
              unsigned int level_height);

    void draw(Sprite_Renderer& renderer);

    bool isCompleted();

private:
    void init(std::vector<std::vector<unsigned int>> tile_data,
              unsigned int                           level_width,
              unsigned int                           level_height);
};
