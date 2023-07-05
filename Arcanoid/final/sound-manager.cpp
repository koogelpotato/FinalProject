#include "sound-manager.hpp"
#include <functional>
#include <map>


static std::mutex        audio_mutex{};

audio_buffer::audio_buffer(const std::string_view audio_file_name,
                           const SDL_AudioSpec&   desired_audio_spec)
{
    SDL_RWops* rwop_ptr_file = SDL_RWFromFile(audio_file_name.data(), "rb");

    SDL_AudioSpec audio_spec{};

    // Load the audio data of a WAVE file into memory.
    SDL_AudioSpec* music_spec =
        SDL_LoadWAV_RW(rwop_ptr_file, 1, &audio_spec, &buffer, &size);

    if (audio_spec.freq != desired_audio_spec.freq ||
        audio_spec.channels != desired_audio_spec.channels ||
        audio_spec.format != desired_audio_spec.format)
    {
        Uint8*    new_converted_buffer{ nullptr };
        int       new_length{};
        const int status = SDL_ConvertAudioSamples(audio_spec.format,
                                                   audio_spec.channels,
                                                   audio_spec.freq,
                                                   buffer,
                                                   size,
                                                   desired_audio_spec.format,
                                                   desired_audio_spec.channels,
                                                   desired_audio_spec.freq,
                                                   &new_converted_buffer,
                                                   &new_length);

        SDL_free(buffer);
        buffer = new_converted_buffer;
        size   = new_length;
    }
}

void audio_buffer::play(const running_mode mode)
{
    std::lock_guard<std::mutex> lock{ audio_mutex };
    current_position = 0;
    is_running       = true;
    this->mode       = mode;
}
void Sound_Manager::load_sound(std::string_view file,
                               std::string_view name,
                               SDL_AudioSpec    audio_spec)
{
    auto hash             = std::hash<std::string_view>{}(name);
    m_sound_storage[hash] = new audio_buffer(file, audio_spec);
}

void Sound_Manager::play_sound(std::string_view name, bool loop) {
    auto hash             = std::hash<std::string_view>{}(name);
    auto sound = m_sound_storage.find(hash);
    if(sound != m_sound_storage.end())
    {
        sound->second->current_position = 0;
        auto mode =  loop ? audio_buffer::running_mode::for_ever : audio_buffer::running_mode::once;
        sound->second->mode = mode;
        m_sound_queue.insert({sound->first, sound->second});
    }
    
}

void Sound_Manager::stop_sound(std::string_view name) {
    auto hash             = std::hash<std::string_view>{}(name);
    m_sound_queue.erase(hash);
}

const std::map<size_t, audio_buffer*>& Sound_Manager::get_sound_queue() const
{
    return m_sound_queue;
}
