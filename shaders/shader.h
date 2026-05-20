#include <fstream>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

class Shader {
  private:
  public:
    int shaderProgram = 0;

    Shader(const char *vertex_filename, const char *frag_filename);

    Shader();

    void useShader();

    void deleteShader();

    char *loadSource(const char *filename);
};