#include "line.h"

Line::Line(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 color) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    vertices.push_back(pos1);
    colors.push_back(color * 0.2f);

    vertices.push_back(pos2);
    colors.push_back(color);

    initialize({ vertices, colors, {}, {}, {} });
}

void Line::draw(int shaderProgram) {
    bindVAO();
    applyMatrix(shaderProgram);
    glDrawArrays(GL_LINES, 0, bufferCount);
}


Lines::Lines(std::vector<glm::vec3> positions, std::vector<glm::vec3> colors) {
    initialize({ positions , colors });
}

void Lines::draw(int shaderProgram) {
    bindVAO();
    applyMatrix(shaderProgram);
    glDrawArrays(GL_LINES, 0, bufferCount);
}