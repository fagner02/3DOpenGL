#include "plane.h"

Plane::Plane(glm::vec3 pos, float length) {
    initialize(calcPlaneBuffers(pos, length));
}

VAOBuffers Plane::calcPlaneBuffers(glm::vec3 pos, float length) {
    float half = length / 2;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indexes;

    for (int i = -1; i < 2;i += 2) {
        for (int j = -1; j < 2; j += 2) {
            vertices.push_back(glm::vec3(i * half, 0.0, j * half) + pos);
            colors.push_back(glm::vec3(0.8, 0.2, 0.2));
            uvs.push_back(glm::vec2((float)(i == 1), (float)(j == 1)));
        }
    }
    glm::vec3 normal = glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);

    for (int i = 0; i < 4;i++) {
        normals.push_back(normal);
    }
    for (int i = 0; i < 3;i++) {
        indexes.push_back(i);
    }
    for (int i = 1; i < 4;i++) {
        indexes.push_back(i);
    }

    return { vertices, colors, normals, uvs, indexes };
}

void Plane::draw(int shaderProgram) {
    bindVAO();
    applyMatrix(shaderProgram);
    glDrawElements(GL_TRIANGLES, bufferCount, GL_UNSIGNED_INT, 0);
}