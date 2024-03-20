#include "line.h"

Line::Line(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 color) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    vertices.push_back(pos1);
    colors.push_back(color);

    vertices.push_back(pos2);
    colors.push_back(color);

    initialize({ vertices, colors });
}