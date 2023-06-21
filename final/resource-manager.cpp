#include "resource-manager.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "stb_image.h"

std::map<std::string, Texture> Resource_Manager::textures;
std::map<std::string, Shader>  Resource_Manager::shaders;

Shader Resource_Manager::load_shader(const char* vertex_shader_file,
                                     const char* fragment_shader_file,
                                     std::string name)
{
    shaders[name] =
        load_shader_from_file(vertex_shader_file, fragment_shader_file);
    return shaders[name];
}

Shader& Resource_Manager::get_shader(std::string name)
{
    return shaders[name];
}

Texture Resource_Manager::load_texture(const char* file,
                                       std::string name)
{
    textures[name] = load_texture_from_file(file);
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

Shader Resource_Manager::load_shader_from_file(const char* vertex_shader_file,
                                               const char* fragment_shader_file)
{

    std::string vertexCode;
    std::string fragmentCode;
    try
    {

        std::ifstream     vertexShaderFile(vertex_shader_file);
        std::ifstream     fragmentShaderFile(fragment_shader_file);
        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertexShaderFile.rdbuf();
        fragment_shader_stream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode   = vertex_shader_stream.str();
        fragmentCode = fragment_shader_stream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    Shader shader;
    shader.compile_shader(vShaderCode, fShaderCode);
    return shader;
}

Texture Resource_Manager::load_texture_from_file(const char* file)
{

    Texture texture;

    int            width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    if (data == nullptr)
    {
        // TODO: Better error handling
        assert(false);
        return texture;
    }

    switch (nrChannels)
    {
        case 1: texture.internal_format = texture.image_format = GL_RED; break;
        case 2: texture.internal_format = texture.image_format = GL_RG; break;
        case 3: texture.internal_format = texture.image_format = GL_RGB; break;
        case 4: texture.internal_format = texture.image_format = GL_RGBA; break;
        default: assert(false); return texture; // TODO: Better error handling
    }

    texture.generate(width, height, data);

    stbi_image_free(data);
    return texture;
}
