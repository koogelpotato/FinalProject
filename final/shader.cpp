#include <iostream>

#include "shader.hpp"

Shader& Shader::use_current_shader()
{
    glUseProgram(this -> ID);
    return *this;
}

void Shader::compile_shader(const char* vertex_source, const char*  fragment_source)
{
    GLuint vertex_shader,fragment_shader;
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source,NULL);
    glCompileShader(vertex_shader);
    check_compile_errors(vertex_shader, "VERTEX");

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,&fragment_source,NULL);
    glCompileShader(fragment_shader);
    check_compile_errors(fragment_shader, "FRAGMENT");

    this -> ID = glCreateProgram();
    glAttachShader(ID,vertex_shader);
    glAttachShader(ID,fragment_shader);

    glLinkProgram(this ->  ID);
    check_compile_errors(this -> ID, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::set_float(const char *name, float value, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::set_integer(const char *name, int value, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::set_vector2f(const char *name, float x, float y, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::set_vector2f(const char *name, const glm::vec2 &value, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::set_vector3f(const char *name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::set_vector3f(const char *name, const glm::vec3 &value, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::set_vector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::set_vector4f(const char *name, const glm::vec4 &value, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::set_matrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
    if (useShader)
        this->use_current_shader();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::check_compile_errors(GLuint shader, std::string type)
{
    int success;
    char info_log[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cerr << " ERROR::SHADER: Compile-time error: Type: " << type << "\n" << info_log << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cerr << "ERROR::SHADER: Link-time error: Type: " << type << "\n"<< info_log << std::endl;
        }
    }
}