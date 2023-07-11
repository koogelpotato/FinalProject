#include "resource-manager.hpp"

#include <SDL3/SDL.h>
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
        SDL_RWops* vertexShaderFile = SDL_RWFromFile(vertex_shader_file, "r");
        SDL_RWops* fragmentShaderFile = SDL_RWFromFile(fragment_shader_file, "r");

        if (vertexShaderFile == nullptr || fragmentShaderFile == nullptr)
        {
            std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
            return Shader(); // Return an empty shader or handle the error appropriately
        }

        // Get the file size
        Sint64 vertexFileSize = SDL_RWsize(vertexShaderFile);
        Sint64 fragmentFileSize = SDL_RWsize(fragmentShaderFile);

        // Allocate memory for the file contents
        char* vertexFileData = new char[vertexFileSize + 1];
        char* fragmentFileData = new char[fragmentFileSize + 1];

        // Read the file contents into memory
        SDL_RWread(vertexShaderFile, vertexFileData, vertexFileSize);
        SDL_RWread(fragmentShaderFile, fragmentFileData, fragmentFileSize);

        // Null-terminate the file contents
        vertexFileData[vertexFileSize] = '\0';
        fragmentFileData[fragmentFileSize] = '\0';

        // Close the file handles
        SDL_RWclose(vertexShaderFile);
        SDL_RWclose(fragmentShaderFile);

        vertexCode = vertexFileData;
        fragmentCode = fragmentFileData;

        // Free the allocated memory
        delete[] vertexFileData;
        delete[] fragmentFileData;
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

    SDL_RWops* fileRW = SDL_RWFromFile(file, "rb");

    if (fileRW == nullptr)
    {
        // TODO: Better error handling
        assert(false);
        return texture;
    }

    // Get the file size
    Sint64 fileSize = SDL_RWsize(fileRW);

    // Allocate memory for the file contents
    unsigned char* fileData = new unsigned char[fileSize];

    // Read the file contents into memory
    SDL_RWread(fileRW, fileData, fileSize);

    // Close the file handle
    SDL_RWclose(fileRW);

    int width, height, nrChannels;
    unsigned char* data = stbi_load_from_memory(fileData, fileSize, &width, &height, &nrChannels, 0);

    if (data == nullptr)
    {
        // TODO: Better error handling
        assert(false);
        delete[] fileData;
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
    delete[] fileData;

    return texture;
}
