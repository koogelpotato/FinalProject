#include "game.hpp"
#include "game-object.hpp"
#include "resource-manager.hpp"
#include "sprite-renderer.hpp"

Sprite_Renderer* renderer;
Game_Object*     player;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE)
    , keyStates()
    , width(width)
    , height(height)
{
}

Game::~Game()
{
    delete renderer;
    delete player;
}

void Game::init_game()
{

    Resource_Manager::load_shader("resources/shaders/sprite.vs",
                                  "resources/shaders/sprite.frag",
                                  "sprite");

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<float>(this->width),
                                      static_cast<float>(this->height),
                                      0.0f,
                                      -1.0f,
                                      1.0f);
    Resource_Manager::get_shader("sprite").use_current_shader().set_integer(
        "image", 0);
    Resource_Manager::get_shader("sprite").set_matrix4("projection",
                                                       projection);

    renderer = new Sprite_Renderer(Resource_Manager::get_shader("sprite"));

    
    Resource_Manager::load_texture("resources/textures/block.png", true, "block");
    Resource_Manager::load_texture("resources/textures/paddle.png", true, "paddle");
    Resource_Manager::load_texture("resources/textures/skull.png", true, "skull");
    //Resource_Manager::load_texture("resources/textures/background_scaled.jpg", true, "background");
    Game_Level level_1;
    level_1.load("resources/levels/one.lvl", this->width, this->height / 2);

    this->levels.push_back(level_1);

    this->current_level = 0;

    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                    this->height - PLAYER_SIZE.y);
    player = new Game_Object(playerPos, PLAYER_SIZE, Resource_Manager::get_texture("paddle"));
}

void Game::update(float dt)
{
    // TO DO add update
}

void Game::proccess_input(SDL_Event event, bool is_key_down)
{
    keyStates[event.key.keysym.sym] = is_key_down;
}
void Game::update_player_input(float dt)
{
    float velocity = PLAYER_VELOCITY * dt;

    if (keyStates[SDLK_a])
    {
        if (player->position.x >= 0.0f)
        {
            player->position.x -= velocity;
            player->rotation = 270.0f;
        }
    }

    if (keyStates[SDLK_d])
    {
        if (player->position.x <= this->width - player->size.x)
        {
            player->position.x += velocity;
            player->rotation = 90.0f;
        }
    }

    if (keyStates[SDLK_w])
    {
        if (player->position.y >= 0.0f)
        {
            player->position.y -= velocity;
            player->rotation = 0.0f;
        }
    }

    if (keyStates[SDLK_s])
    {
        if (player->position.y <= this->height - player->size.y)
        {
            player->position.y += velocity;
            player->rotation = 180.0f;
        }
    }
}

void Game::render()
{

    if (this->State == GAME_ACTIVE)
    {
        //renderer->draw_sprite(Resource_Manager::get_texture("background"), glm::vec2(0.0f,0.0f),glm::vec2(this->width, this->height));
        this->levels[this->current_level].draw(*renderer);
        player->draw(*renderer);
    }
}