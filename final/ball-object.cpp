#include "ball-object.hpp"

Ball_Object::Ball_Object():Game_Object(), radius(12.5f), is_stuck(true){}

Ball_Object::Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite):
    Game_Object(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f),velocity),radius(radius),is_stuck(true){}

glm::vec2 Ball_Object::move(float dt, unsigned int window_width)
{
    if(!this -> is_stuck)
    {
        this->position += this->velocity * dt;

        if(this->position.x <= 0)
        {
            this->velocity.x = -this->velocity.x;
            this->position.x = 0.0f;
        }
        else if(this->position.x + this-> size.x >= window_width)
        {
            this->velocity.x = -this->velocity.x;
            this->position.x = window_width - this->size.x;
        }
        if(this->position.y <= 0.0f)
        {
            this->velocity.y = -this->velocity.y;
            this->position.y = 0.0f;
        }
    }
    return this->position;
}

void Ball_Object::reset(glm::vec2 position, glm::vec2)
{
    this->position = position;
    this->velocity = velocity;
    this->is_stuck = true;
}