#include "line.h"

Line::Line(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 color) {
    std::vector<float> vertices;
    std::vector<float> colors;
    for (int i = 0; i < 3; i++) {
        vertices.push_back(pos1[i]);
        colors.push_back(color[i]);
    }
    for (int i = 0; i < 3; i++) {
        vertices.push_back(pos2[i]);
        colors.push_back(color[i]);
    }
    defaultLateConstructor(vertices, colors);
}