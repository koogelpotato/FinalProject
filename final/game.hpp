#pragma once

#include "glad/glad.h"
#include <SDL3/SDL.h>
#include "glm/glm.hpp"

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_EXIT
};

const glm::vec2 PLAYER_SIZE(360.0f, 360.0f);
const float PLAYER_VELOCITY(100.0f);

class Game{
    public:
    GameState State;
    bool keys[1024];
    unsigned int width, height;
    Game(unsigned int width, unsigned int height);
    ~Game();
    void init_game();
    void proccess_input(float dt);
    void update(float dt);
    void render();
};