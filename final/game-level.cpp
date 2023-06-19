#include <fstream>
#include <sstream>

#include "game-level.hpp"

void Game_Level::load(const char*  file,
                      unsigned int level_width,
                      unsigned int level_height)
{
    this->bricks.clear();

    unsigned int tile_code;

    Game_Level level;

    std::string line;

    std::ifstream                          fstream(file);
    std::vector<std::vector<unsigned int>> tile_data;
    if (fstream)
    {
        while (std::getline(fstream, line))
        {
            std::istringstream        sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tile_code)
                row.push_back(tile_code);
            tile_data.push_back(row);
        }
        if (tile_data.size() > 0)
            this->init(tile_data, level_width, level_height);
    }
}

void Game_Level::draw(Sprite_Renderer& renderer)
{
    for (Game_Object& tile : this->bricks)
        if (!tile.destroyed)
            tile.draw(renderer);
}

bool Game_Level::isCompleted()
{
    for (Game_Object& tile : this->bricks)
        if (!tile.is_solid && tile.destroyed)
            return false;
    return true;
}

void Game_Level::init(std::vector<std::vector<unsigned int>> tile_data,
                      unsigned int                           level_width,
                      unsigned int                           level_height)
{
    unsigned int height     = tile_data.size();
    unsigned int width      = tile_data[0].size();
    float        unit_width = level_width / static_cast<float>(width),
          unit_height       = level_height / height;

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (tile_data[y][x] == 1)
            {
                glm::vec2   pos(unit_height * x, unit_height * y);
                glm::vec2   size(unit_width, unit_height);

                Game_Object obj(pos,
                                size,
                                Resource_Manager::get_texture(
                                    "block"),
                                glm::vec3(0.8f, 0.8f, 0.7f));
                obj.is_solid = true;
                this->bricks.push_back(obj);
            }
            else if (tile_data[y][x] > 1)
            {
                glm::vec3 color = glm::vec3(1.0f);
                switch (tile_data[y][x])
                {
                    case 2:
                        color = glm::vec3(0.2f, 0.6f, 1.0f);
                        break;
                    case 3:
                        color = glm::vec3(0.0f, 0.7f, 0.0f);
                        break;
                    case 4:
                        color = glm::vec3(0.8f, 0.8f, 0.4f);
                        break;
                    case 5:
                        color = glm::vec3(1.0f, 0.5f, 0.0f);
                        break;
                    default:
                        std::cout << "error, no such block found" << std::endl;
                        break;
                }

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->bricks.push_back(Game_Object(
                    pos, size, Resource_Manager::get_texture("block"), color));
            }
        }

    }
}