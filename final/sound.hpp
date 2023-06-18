#pragma once

#include <SDL3/SDL.h>
#include <chrono>
#include <cmath>

struct audio_buffer
{
    uint8_t* start       = nullptr;
    size_t   size        = 0;
    size_t   current_pos = 0;

    struct
    {
        size_t frequency = 0;
        double time      = 0.0;
        bool   use_note  = false;
    } note;
};

enum stereo_audio_channel
{
    left,
    right,
    left_and_right
};
