#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "texture.hpp"
#include "shader.hpp"

class Sprite_Renderer
{
    public:
        Sprite_Renderer(Shader &shader);
        
        ~Sprite_Renderer();

        void draw_sprite(Texture &texture, glm::vec2 position, glm::vec2  size = glm::vec2(10.0f,10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    private:
        Shader shader;

        GLuint quadVAO;

        void init_render_data();
};