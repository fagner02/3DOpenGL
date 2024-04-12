#pragma once
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "./include/GL/glut.h"
#include "./include/GL/glext.h"
#include "./glm/glm.hpp"
#include "vertex_array.h"

struct PixelInfo {
    unsigned int ObjectID = 0;
    unsigned int DrawID = 0;
    unsigned int PrimID = 0;
    unsigned int _;
};

class Picking {
public:
    std::vector<VertexArray*> objs;
    Picking() {}

    ~Picking();

    void init(unsigned int WindowWidth, unsigned int WindowHeight);

    void enableWriting();

    void disableWriting();

    void applyIndex(int shaderProgram, int index);

    PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:

    unsigned int fbo = 0;
    unsigned int rbo = 0;
    GLuint pickingTexture = 0;
    GLuint depthTexture = 0;
};