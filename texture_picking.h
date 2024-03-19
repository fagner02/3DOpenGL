#pragma once
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "./include/GL/glut.h"
#include "./include/GL/glext.h"
#include "./glm/glm.hpp"

struct PixelInfo {
    unsigned int ObjectID = 0;
    unsigned int DrawID = 0;
    unsigned int PrimID = 0;
    unsigned int _;

    void Print() {
        printf("Object %d draw %d prim %d\n", ObjectID, DrawID, PrimID);
    }
};

class Picking {
public:
    Picking() {}

    ~Picking();

    void init(unsigned int WindowWidth, unsigned int WindowHeight);

    void enableWriting();

    void disableWriting();
    void applyIndex(int shaderProgram, int index);

    PixelInfo ReadPixel(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

private:
    unsigned int fbo = 0;
    unsigned int rbo = 0;
    GLuint pickingTexture = 0;
    GLuint depthTexture = 0;
};