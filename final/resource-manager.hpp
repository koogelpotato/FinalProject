#pragma once

#include <map>
#include <string>

#include "glad/glad.h"

#include "shader.hpp"
#include "texture.hpp"
#include "sound.hpp"

class Resource_Manager
{
public:
    static std::map<std::string, Shader>  shaders;
    static std::map<std::string, Texture> textures;
    static std::map<std::string, Sound> sounds;

    static Shader load_shader(const char* vertex_shader_file,
                              const char* fragment_shader_file,
                              std::string name);

    static Shader& get_shader(std::string name);

    static Texture load_texture(const char* file, std::string name);

    static Texture& get_texture(std::string name);

    static Sound load_sound(const char* file, std::string name, uint32_t device_, SDL_AudioSpec audio_spec);

    static Sound& get_sound(std::string name);

    static void clear();

private:
    Resource_Manager() {}

    static Shader load_shader_from_file(const char* vertex_shader_file,
                                        const char* fragment_shader_file);

    static Texture load_texture_from_file(const char* file);

    static Sound load_sound_from_file(const char* file, uint32_t device_, SDL_AudioSpec audio_spec);

};