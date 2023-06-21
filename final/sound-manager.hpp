#pragma once
#include <SDL3/SDL_audio.h>
#include <queue> 
#include <string_view>
#include <mutex>
#include <map>

struct audio_buffer
{
    enum class running_mode
    {
        once,
        for_ever
    };

    Uint8*       buffer{ nullptr };
    Uint32       size{};
    std::size_t  current_position{};
    running_mode mode{ running_mode::once };
    bool         is_running{ false };

    audio_buffer(const std::string_view audio_file_name,
                 const SDL_AudioSpec&   desired_audio_spec);
    ~audio_buffer() { SDL_free(buffer); };

    void play(const running_mode mode);
};

class Sound_Manager {
public:
    Sound_Manager(){};

    void load_sound(std::string_view file,std::string_view name , SDL_AudioSpec audio_spec);
    void play_sound(std::string_view name, bool loop);
    void stop_sound(std::string_view name);
    const std::map<size_t, audio_buffer*>& get_sound_queue() const;

private:
    std::map<size_t, audio_buffer*> m_sound_queue;
    std::map<size_t, audio_buffer*> m_sound_storage;

    audio_buffer load_sound_from_file(const char* file, uint32_t device_, SDL_AudioSpec audio_spec);
};