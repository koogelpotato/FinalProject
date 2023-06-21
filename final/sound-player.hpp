#pragma once
#include "sound.hpp"
#include <queue> 

class Sound_Player {
public:
    Sound_Player();

    void load_sound(const Sound& sound);
    void play_sound(Sound sound, bool loop = false);
    void stop_sound(Sound sound);
    void enqueue_sound(Sound sound);
    void play_next_sound();

private:
    std::queue<Sound> sound_queue;
};