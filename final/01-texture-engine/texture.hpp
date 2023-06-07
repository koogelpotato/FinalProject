#pragma once

#include "glad/glad.h"

class Texture
{
    public:
        GLuint ID;
        GLuint width, height;
        GLuint internal_format;
        GLuint image_format; //supports many image formats depending on the int value
        GLuint wrap_s; //support different axis wrappings
        GLuint wrap_t;
        GLuint filter_min; //different filtering types
        GLuint filter_max;

        Texture();

        void generate(GLuint width, GLuint height, unsigned char* data);

        void bind() const;

};