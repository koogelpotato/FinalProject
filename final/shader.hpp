#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
    GLuint  ID;

    Shader() {};

    Shader &use_current_shader();
    
    void compile_shader(const char *vertex_source, const char *fragment_source);
    
    void    set_float    (const char *name, float value, bool use_shader = false);
    
    void    set_integer  (const char *name, int value, bool use_shader = false);
    
    void    set_vector2f (const char *name, float x, float y, bool use_shader = false);
    
    void    set_vector2f (const char *name, const glm::vec2 &value, bool use_shader = false);
    
    void    set_vector3f (const char *name, float x, float y, float z, bool use_shader = false);
    
    void    set_vector3f (const char *name, const glm::vec3 &value, bool use_shader = false);
    
    void    set_vector4f (const char *name, float x, float y, float z, float w, bool use_shader = false);
    
    void    set_vector4f (const char *name, const glm::vec4 &value, bool useShader = false);
    
    void    set_matrix4  (const char *name, const glm::mat4 &matrix, bool useShader = false);
    
    private:

    void check_compile_errors(GLuint shader, std::string type);
};
