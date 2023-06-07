#include "resource-manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"


std::map<std::string, Texture>    Resource_Manager::textures;
std::map<std::string, Shader>       Resource_Manager::shaders;


Shader Resource_Manager::load_shader(const char *vertex_shader_file, const char *fragment_shader_file, std::string name)
{
    shaders[name] = load_shader_from_file(vertex_shader_file, fragment_shader_file);
    return shaders[name];
}

Shader& Resource_Manager::get_shader(std::string name)
{
    return shaders[name];
}

Texture Resource_Manager::load_texture(const char *file, bool alpha, std::string name)
{
    textures[name] = load_texture_from_file(file, alpha);
    return textures[name];
}

Texture& Resource_Manager::get_texture(std::string name)
{
    return textures[name];
}

void Resource_Manager::clear()
{
    	
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);
    
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader Resource_Manager::load_shader_from_file(const char *vertex_shader_file, const char *fragment_shader_file)
{
    
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        
        std::ifstream vertexShaderFile(vertex_shader_file);
        std::ifstream fragmentShaderFile(fragment_shader_file);
        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertexShaderFile.rdbuf();
        fragment_shader_stream << fragmentShaderFile.rdbuf();
        
        vertexShaderFile.close();
        fragmentShaderFile.close();
        
        vertexCode = vertex_shader_stream.str();
        fragmentCode = fragment_shader_stream.str();
        
        
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    
    Shader shader;
    shader.compile_shader(vShaderCode, fShaderCode);
    return shader;
}

Texture Resource_Manager::load_texture_from_file(const char *file, bool alpha)
{
    
    Texture texture;
    if (alpha)
    {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    
    texture.generate(width, height, data);
    
    stbi_image_free(data);
    return texture;
}
