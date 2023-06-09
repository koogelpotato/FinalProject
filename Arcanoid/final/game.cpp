#include "game.hpp"
#include "ball-object.hpp"
#include "game-object.hpp"
#include "resource-manager.hpp"
#include "sprite-renderer.hpp"

const glm::vec2 PLAYER_SIZE(150.0f, 20.0f);
const float     PLAYER_VELOCITY(10.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(1.5f, -1.5f);
const float     BALL_RADIUS = 12.5f;

Sprite_Renderer* renderer;
Game_Object*     player;
Ball_Object*     ball;

static std::mutex audio_mutex{};

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

void init_audio();

void sdl_audio_callback(void* userdata, Uint8* stream, int len)
{
    std::lock_guard<std::mutex> lock{ audio_mutex };

    std::memset(stream, 0, len);

    Sound_Manager* audio_manager = static_cast<Sound_Manager*>(userdata);

#if defined(WIN32) || defined(_WIN32) ||                                       \
    defined(__WIN32) && !defined(__CYGWIN__)
    constexpr int32_t AUDIO_FORMAT = AUDIO_F32LSB;
#else
    constexpr int32_t AUDIO_FORMAT = SDL_AUDIO_S16LSB;
#endif

    for (auto it : audio_manager->get_sound_queue())
    {
        auto& sound = it.second;

        std::size_t stream_len{ static_cast<std::size_t>(len) };

        const Uint8* start_buffer = sound->buffer + sound->current_position;

        const std::size_t bytes_left_in_buffer =
            sound->size - sound->current_position;

        if (bytes_left_in_buffer <= stream_len)
        {
            SDL_MixAudioFormat(stream,
                               start_buffer,
                               AUDIO_FORMAT,
                               bytes_left_in_buffer,
                               SDL_MIX_MAXVOLUME);
            sound->current_position += bytes_left_in_buffer;
        }
        else
        {
            SDL_MixAudioFormat(stream,
                               start_buffer,
                               AUDIO_FORMAT,
                               stream_len,
                               SDL_MIX_MAXVOLUME);
            sound->current_position += stream_len;
        }
        if (sound->current_position == sound->size)
        {
            audio_manager->stop_sound("theme");
        }
    }
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
    // Resource_Manager::load_texture("resources/textures/logo.png", "logo");
    Game_Level level_1;
    level_1.load("resources/levels/one.lvl", this->width, this->height / 2);
    Game_Level level_2;
    level_2.load("resources/levels/two.lvl", this->width, this->height / 2);

    this->levels.push_back(level_1);
    this->levels.push_back(level_2);
    this->current_level = 0;

    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                    this->height - PLAYER_SIZE.y);
    player              = new Game_Object(
        playerPos, PLAYER_SIZE, Resource_Manager::get_texture("paddle"));

    glm::vec2 ballPos =
        playerPos +
        glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

    ball = new Ball_Object(ballPos,
                           BALL_RADIUS,
                           INITIAL_BALL_VELOCITY,
                           Resource_Manager::get_texture("ball"));

    SDL_AudioSpec desired_spec;
    desired_spec.freq     = 44100;
    desired_spec.format   = SDL_AUDIO_S16;
    desired_spec.channels = 2;
    desired_spec.samples  = 512;
    desired_spec.callback = sdl_audio_callback;

    desired_spec.userdata = &m_audio_player;
    device                = SDL_OpenAudioDevice(
        NULL, 0, &desired_spec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (device == 0)
    {
        printf("Failed to open audio device! SDL_Error: %s\n", SDL_GetError());
    }

    m_audio_player.load_sound(
        "resources/sounds/through-space.wav", "theme", desired_spec);
    m_audio_player.play_sound("theme", true);
    // audio_buffer sound_p
    SDL_PlayAudioDevice(device);
}

void Game::update(float dt)
{
    ball->move(dt, this->width);
    this->perform_collisions();

    if (ball->position.y >= this->height)
    {
        this->reset_level();
        this->reset_player();
    }
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
            if (ball->is_stuck)
                ball->position.x -= velocity;
        }
    }

    if (keyStates[SDLK_d])
    {
        if (player->position.x <= this->width - player->size.x)
        {
            player->position.x += velocity;
            if (ball->is_stuck)
                ball->position.x += velocity;
        }
    }
    if (keyStates[SDLK_SPACE])
    {
        ball->is_stuck = false;
    }
}

void Game::render()
{

    if (this->State == GAME_ACTIVE)
    {
        // renderer->draw_sprite(Resource_Manager::get_texture("background"),
        // glm::vec2(0.0f,0.0f),glm::vec2(this->width, this->height));
        this->levels[this->current_level].draw(*renderer);
        player->draw(*renderer);
        ball->draw(*renderer);
    }
}

void Game::reset_level()
{
    if (this->current_level == 0)
        this->levels[0].load(
            "resources/levels/one.lvl", this->width, this->height / 2);
    else if (this->current_level == 1)
        this->levels[1].load(
            "resources/levels/two.lvl", this->width, this->height / 2);
}

void Game::reset_player()
{
    player->size     = PLAYER_SIZE;
    player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                 this->height - PLAYER_SIZE.y);
    ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
                                             -(BALL_RADIUS * 2.0f)),
                INITIAL_BALL_VELOCITY);
}

bool      check_collision(Game_Object& one, Game_Object& two);
collision check_collision(Ball_Object& one, Game_Object& two);
direction vector_direction(glm::vec2 target);

void Game::perform_collisions()
{
    for (Game_Object& box : this->levels[this->current_level].bricks)
    {
        if (!box.destroyed)
        {
            collision col = check_collision(*ball, box);
            if (std::get<0>(col))
            {
                if (!box.is_solid)
                    box.destroyed = true;

                direction dir         = std::get<1>(col);
                glm::vec2 diff_vector = std::get<2>(col);
                if (dir == LEFT || dir == RIGHT)
                {
                    ball->velocity.x = -ball->velocity.x;

                    float penetration = ball->radius - std::abs(diff_vector.x);

                    if (dir == LEFT)
                        ball->position.x += penetration;
                    else
                        ball->position.x -= penetration;
                }
                else
                {
                    ball->velocity.y = -ball->velocity.y;

                    float penetration = ball->radius - std::abs(diff_vector.y);

                    if (dir == UP)
                        ball->position.y -= penetration;
                    else
                        ball->position.y += penetration;
                }
            }
        }
    }
    collision result = check_collision(*ball, *player);
    if (!ball->is_stuck && std::get<0>(result))
    {
        float     center_board = player->position.x + player->size.x / 2.0f;
        float     distance   = (ball->position.x + ball->radius) - center_board;
        float     percentage = distance / (player->size.x / 2.0f);
        float     strength   = 2.0f;
        glm::vec2 old_velocity = ball->velocity;
        ball->velocity =
            glm::normalize(ball->velocity) * glm::length(old_velocity);
        ball->velocity.y = -1.0f * abs(ball->velocity.y);
    }
}

bool check_collision(Game_Object& one, Game_Object& two)
{

    bool collisionX = one.position.x + one.size.x >= two.position.x &&
                      two.position.x + two.size.x >= one.position.x;

    bool collisionY = one.position.y + one.size.y >= two.position.y &&
                      two.position.y + two.size.y >= one.position.y;

    return collisionX && collisionY;
}

collision check_collision(Ball_Object& one, Game_Object& two)
{
    glm::vec2 center(one.position + one.radius);
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x,
                          two.position.y + aabb_half_extents.y);

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped =
        glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference        = closest - center;
    if (glm::length(difference) < one.radius)
        return std::make_tuple(true, vector_direction(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

direction vector_direction(glm::vec2 target)
{
    glm::vec2 compass[] = { glm::vec2(0.0f, 1.0f),
                            glm::vec2(1.0f, 0.0f),
                            glm::vec2(0.0f, -1.0f),
                            glm::vec2(-1.0f, 0.0f) };

    float        max        = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max        = dot_product;
            best_match = i;
        }
    }

    return (direction)best_match;
}

void Game::deinit()
{
    SDL_CloseAudioDevice(device);
}
