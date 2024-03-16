#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "../include/GL/glut.h"
#include "../include/GL/glext.h"


class Shader {
private:

public:
    int shaderProgram = 0;

    Shader(char* vertex_filename, char* frag_filename);

    Shader();

    void useShader();

    void deleteShader();
};