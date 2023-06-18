#pragma once

#include "game-level.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <SDL3/SDL.h>

#include <map>

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_EXIT
};

const glm::vec2 PLAYER_SIZE(150.0f, 20.0f);
const float     PLAYER_VELOCITY(100.0f);

class Game
{
public:
    std::vector<Game_Level> levels;
    unsigned int            current_level;
    GameState               State;
    std::map<int, bool>     keyStates;
    unsigned int            width, height;
    Game(unsigned int width, unsigned int height);
    ~Game();
    void init_game();
    void proccess_input(SDL_Event event, bool is_key_down);
    void update_player_input(float dt);
    void update(float dt);
    void render();
};