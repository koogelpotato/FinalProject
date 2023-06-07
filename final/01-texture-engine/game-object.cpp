#include "game-object.hpp"

Game_Object::Game_Object()
    :  position(0.0f, 0.0f), size(1.0f,1.0f), velocity(0.0f), color(1.0f),rotation(0.0f), sprite(), is_solid(false), destroyed(false){}

Game_Object::Game_Object(glm::vec2 pos, glm::vec2 size,  Texture sprite, glm::vec3 color, glm::vec2 velocity)
    : position(pos),  size(size), velocity(velocity), sprite(sprite), color(color), rotation(0.0), is_solid(false), destroyed(false){}

void Game_Object::Draw(Sprite_Renderer &renderer)
{
    renderer.draw_sprite(this->sprite, this->position, this->size, this->rotation, this->color);
}