#pragma once
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "./include/GL/glut.h"
#include "./include/GL/glext.h"
#include "./include/glm/glm.hpp"
#include "./include/glm/gtc/type_ptr.hpp"

struct VAOBuffers {
    std::vector<glm::vec3> coordinates;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoord;
    std::vector<unsigned int> indexes;
};

class VertexArray {
protected:
    void initialize(VAOBuffers buffers);
private:
    template<typename T>
    void bindAttrib(std::vector<T> buffer, int index, int size);
    unsigned int vao, vbo[4], ebo;
    int attrNum;
public:
    glm::mat4 modelMatrix = glm::mat4(1.0);
    unsigned int bufferCount;
    VertexArray(VAOBuffers buffers);
    VertexArray();
    void bindVAO();
    void enableAttribs();
    void applyMatrix(int shaderProgram);
};