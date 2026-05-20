#pragma once
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

class Texture {
  public:
    const char *filename;
    unsigned int textureId = 0;
    Texture(const char *filename);
    void bindTexture(int shaderProgram);
};