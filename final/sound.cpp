#include "sound.hpp"

void Sound::generate_sound(const char* path, uint32_t device_, SDL_AudioSpec audio_spec) {
    buffer = new audio_buffer(path, device_, audio_spec);
}