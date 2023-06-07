#include "sprite-renderer.hpp"

Sprite_Renderer::Sprite_Renderer(Shader& shader)
{
    this->shader = shader;
    this->init_render_data();
}

Sprite_Renderer::~Sprite_Renderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Sprite_Renderer::draw_sprite(Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    this->shader.use_current_shader();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position,0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x,0.5f * size.y,  0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f,0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f *  size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size,1.0f));

    this->shader.set_matrix4("model", model);
    this->shader.set_vector3f("color", color);

    glActiveTexture(GL_TEXTURE0);

    texture.bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite_Renderer::init_render_data()
{
    GLuint VBO;

    float vertices[] = { 
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1,&this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this -> quadVAO);
    glEnableVertexAttribArray(0);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
