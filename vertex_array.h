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

class VertexArray {
protected:
    void defaultLateConstructor(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals = std::vector<float>());
    void indexLateConstructor(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals, std::vector<unsigned int> indexes);
private:
    void bindAttrib(std::vector<float> buffer, int index);
    void initialize(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals);
    unsigned int vao, vbo[3], ebo;
public:
    glm::mat4 modelMatrix = glm::mat4(1.0);
    unsigned int bufferCount;
    VertexArray(std::vector<float> coordinates, std::vector<float> colors);
    VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals);
    VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals, std::vector<unsigned int> indexes);
    VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<unsigned int> indexes);
    VertexArray();
    void bindVAO();
    void enableAttribs();
    void applyMatrix(int shaderProgram);
};