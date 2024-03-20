#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <GL/glext.h>
#include <IL/il.h>

class Texture {
    unsigned int texture;
public:
    Texture(const char* filename);
    void bindTexture();
};