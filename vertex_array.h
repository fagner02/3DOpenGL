#pragma once
#include <iostream>
#include <vector>
#include "./include/GL/glut.h"
#include "./include/GL/glext.h"

class VertexArray {
protected:
    void defaultLateConstructor(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals);
    void indexLateConstructor(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals, std::vector<unsigned int> indexes);
private:
    void bindAttrib(std::vector<float> buffer, int index);
    void initialize(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals);
    unsigned int vao, vbo[3], ebo;
public:
    unsigned int bufferCount;
    VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals);
    VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals, std::vector<unsigned int> indexes);
    VertexArray();
    void bindVAO();
    void enableAttribs();
};