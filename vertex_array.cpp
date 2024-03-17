#include "vertex_array.h"

VertexArray::VertexArray() {}

void VertexArray::bindAttrib(std::vector<float> buffer, int index) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDisableVertexAttribArray(index);
}

void VertexArray::initialize(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(normals.size() == 0 ? 2 : 3, vbo);
    glBindVertexArray(vao);

    this->bindAttrib(coordinates, 0);
    this->bindAttrib(colors, 1);
    if (normals.size() > 0)this->bindAttrib(normals, 2);
}

void VertexArray::defaultLateConstructor(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals) {
    initialize(coordinates, colors, normals);
    bufferCount = coordinates.size() / 3;
}

void VertexArray::indexLateConstructor(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals, std::vector<unsigned int> indexes) {
    initialize(coordinates, colors, normals);
    bufferCount = indexes.size();
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned int), indexes.data(), GL_DYNAMIC_DRAW);
}

VertexArray::VertexArray(std::vector<float> coordinates, std::vector<float> colors) {
    defaultLateConstructor(coordinates, colors);
}

VertexArray::VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals) {
    defaultLateConstructor(coordinates, colors, normals);
}

VertexArray::VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<float> normals, std::vector<unsigned int> indexes) {
    indexLateConstructor(coordinates, colors, normals, indexes);
}

VertexArray::VertexArray(std::vector<float> coordinates, std::vector<float> colors, std::vector<unsigned int> indexes) {
    indexLateConstructor(coordinates, colors, std::vector<float>(), indexes);
}

void VertexArray::enableAttribs() {
    for (int i = 0; i < 3;i++) {
        glEnableVertexAttribArray(i);
    }
}

void VertexArray::bindVAO() {
    glBindVertexArray(vao);
    enableAttribs();
}

