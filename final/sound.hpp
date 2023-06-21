#pragma once

#include "audio-buffer.hpp"
class Sound {
    public:
        Sound() {}
        void generate_sound(const char* path, uint32_t device_, SDL_AudioSpec audio_spec);
        
        audio_buffer* buffer;
    };