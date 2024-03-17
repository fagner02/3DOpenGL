#include "line.h"

Line::Line(glm::vec3 pos1, glm::vec3 pos2) {
    std::vector<float> vertices;
    for (int i = 0; i < 3; i++) {
        vertices.push_back(pos1[i]);
    }
    for (int i = 0; i < 3; i++) {
        vertices.push_back(pos2[i]);
    }
    defaultLateConstructor(vertices, { 0.9, 0.9,0.9, 0.9, 0.9,0.9 });
}