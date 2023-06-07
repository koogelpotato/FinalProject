#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "sprite-renderer.hpp"
#include "texture.hpp"

class Game_Object
{
    public:
    glm::vec2 position, velocity, size; 
    glm::vec3 color;
    float rotation;
    bool is_solid;
    bool destroyed;;

    Texture sprite;
    Game_Object();
    Game_Object(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    virtual void Draw(Sprite_Renderer &renderer);
};