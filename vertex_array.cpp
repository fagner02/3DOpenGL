#include "vertex_array.h"

VertexArray::VertexArray() {}

template<typename T>
void VertexArray::bindAttrib(std::vector<T> buffer, int index, int size) {
    // std::cout << buffer[0].length() << ' ' << sizeof(T) << "\n";
    glBindBuffer(GL_ARRAY_BUFFER, vbo[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDisableVertexAttribArray(index);
}

void VertexArray::initialize(VAOBuffers buffers) {
    bool hasNormals = buffers.normals.size() > 0;
    bool hasColors = buffers.colors.size() > 0;
    bool hasTexCoord = buffers.texCoord.size() > 0;
    glGenVertexArrays(1, &vao);
    attrNum = 1 + hasNormals + hasColors + hasTexCoord;
    glGenBuffers(attrNum, vbo);
    glBindVertexArray(vao);

    this->bindAttrib(buffers.coordinates, 0, 3);

    int index = 1;
    if (hasColors) {
        this->bindAttrib(buffers.colors, index, 3);
        index++;
    }
    if (hasNormals) {
        this->bindAttrib(buffers.normals, index, 3);
        index++;
    }
    if (hasTexCoord) {
        this->bindAttrib(buffers.texCoord, index, 2);
    }

    if (buffers.indexes.size() > 0) {
        bufferCount = buffers.indexes.size();
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffers.indexes.size() * sizeof(unsigned int), buffers.indexes.data(), GL_DYNAMIC_DRAW);
    }
}

VertexArray::VertexArray(VAOBuffers buffers) {
    initialize(buffers);
}

void VertexArray::enableAttribs() {
    for (int i = 0; i < attrNum;i++) {
        glEnableVertexAttribArray(i);
    }
}

void VertexArray::bindVAO() {
    glBindVertexArray(vao);
    enableAttribs();
}

void VertexArray::applyMatrix(int shaderProgram) {
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}