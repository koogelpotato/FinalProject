#include "sound-player.hpp"

Sound_Player::Sound_Player() {}

void Sound_Player::load_sound(const Sound& sound) {
    sound_queue.push(sound);
}

void Sound_Player::play_sound(Sound sound, bool loop) {
    if (loop) {
        SDL_PlayAudioDevice(sound.buffer->device);
    } else {
        SDL_PlayAudioDevice(sound.buffer->device);
    }
}

void Sound_Player::stop_sound(Sound sound) {
    SDL_PauseAudioDevice(sound.buffer->device);
}

void Sound_Player::enqueue_sound(Sound sound) {
    sound_queue.push(sound);
}

void Sound_Player::play_next_sound() {
    if (!sound_queue.empty()) {
        Sound sound = sound_queue.front();
        sound_queue.pop();
        play_sound(sound);
    }
}