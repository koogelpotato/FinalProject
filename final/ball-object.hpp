#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "game-object.hpp"
#include "texture.hpp"

class Ball_Object : public Game_Object
{
    public:
        float radius;
        bool is_stuck;

        Ball_Object();
        Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite);

        glm::vec2 move(float dt, unsigned int window);
        void reset(glm::vec2 position, glm::vec2 velocity);
};