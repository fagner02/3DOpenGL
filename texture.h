#pragma once
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <GL/glext.h>

class Texture {
    unsigned int texture = 0;
public:
    Texture(const char* filename);
    void bindTexture();
};