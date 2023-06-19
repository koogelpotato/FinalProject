#include "game.hpp"
#include "game-object.hpp"
#include "resource-manager.hpp"
#include "sprite-renderer.hpp"
#include "ball-object.hpp"


const glm::vec2 PLAYER_SIZE(150.0f, 20.0f);
const float     PLAYER_VELOCITY(10.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(1.5f, -1.5f);
const float BALL_RADIUS = 12.5f;

Sprite_Renderer* renderer;
Game_Object*     player;
Ball_Object* ball;


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
    delete ball;
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

    
    Resource_Manager::load_texture("resources/textures/block.png", "block");
    Resource_Manager::load_texture("resources/textures/paddle.png", "paddle");
    Resource_Manager::load_texture("resources/textures/circle.png", "ball");
    //Resource_Manager::load_texture("resources/textures/background_scaled.jpg", true, "background");
    Game_Level level_1;
    level_1.load("resources/levels/one.lvl", this->width, this->height / 2);

    this->levels.push_back(level_1);

    this->current_level = 0;

    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                    this->height - PLAYER_SIZE.y);
    player = new Game_Object(playerPos, PLAYER_SIZE, Resource_Manager::get_texture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

    ball = new Ball_Object(ballPos,BALL_RADIUS, INITIAL_BALL_VELOCITY, Resource_Manager::get_texture("ball"));
}

void Game::update(float dt)
{
    ball->move(dt, this->width);
    this->perform_collisions();
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
            if(ball->is_stuck)
                ball->position.x -= velocity;
        }
    }

    if (keyStates[SDLK_d])
    {
        if (player->position.x <= this->width - player->size.x)
        {
            player->position.x += velocity;
            if(ball->is_stuck)
                ball->position.x += velocity;
        }
    }
    if(keyStates[SDLK_SPACE])
    {
        ball->is_stuck = false;
    }
}

void Game::render()
{

    if (this->State == GAME_ACTIVE)
    {
        //renderer->draw_sprite(Resource_Manager::get_texture("background"), glm::vec2(0.0f,0.0f),glm::vec2(this->width, this->height));
        this->levels[this->current_level].draw(*renderer);
        player->draw(*renderer);
        ball->draw(*renderer);
    }
}

bool check_collision(Game_Object &one, Game_Object &two);
collision check_collision(Ball_Object &one, Game_Object &two);
direction vector_direction(glm::vec2 target);

void Game::perform_collisions()
{
    for(Game_Object &box : this->levels[this->current_level].bricks)
    {
        if(!box.destroyed)
        {
            collision col = check_collision(*ball, box);
            if(std::get<0>(col))
            {
                if(!box.is_solid)
                    box.destroyed = true;

                direction dir = std::get<1>(col);
                glm::vec2 diff_vector = std::get<2>(col);
                if(dir == LEFT || dir == RIGHT)
                {
                    ball->velocity.x = -ball->velocity.x;

                    float penetration = ball->radius - std::abs(diff_vector.x);

                    if(dir == LEFT)
                        ball->position.x += penetration;
                    else
                        ball->position.x -= penetration;
                }
                else
                {
                    ball->velocity.y =-ball->velocity.y;

                    float penetration = ball->radius -std::abs(diff_vector.y);

                    if(dir == UP)
                        ball->position.y -= penetration;
                    else
                        ball->position.y += penetration;
                }
            }
        }
    }
    collision result = check_collision(*ball, *player);
    if(!ball->is_stuck && std::get<0>(result))
    {
        float center_board = player->position.x + player->size.x / 2.0f;
        float distance = (ball->position.x + ball->radius) - center_board;
        float percentage = distance / (player->size.x/2.0f);
        float strength = 2.0f;
        glm::vec2 old_velocity = ball->velocity;
        ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity);
        ball->velocity.y = -1.0f * abs(ball->velocity.y);
    }
}

bool check_collision(Game_Object &one, Game_Object &two)
{
   
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
   
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
   
    return collisionX && collisionY;
}

collision check_collision(Ball_Object &one, Game_Object &two)
{
    glm::vec2 center(one.position + one.radius);
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;
    if(glm::length(difference) < one.radius)
        return std::make_tuple(true, vector_direction(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

direction vector_direction(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f), 
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };

    float max = 0.0f;
    unsigned int best_match = -1;
    for(unsigned int i=0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if(dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }

    return (direction)best_match;
}