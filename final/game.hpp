#pragma once

#include "game-level.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include "sound-manager.hpp"

#include <map>

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_EXIT
};


enum direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool,direction, glm::vec2> collision;

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
    void perform_collisions();
    void reset_level();
    void reset_player();
    void deinit();
private:
    uint32_t device;
    Sound_Manager m_audio_player;
};