#include "extrude_point.h"

glm::vec3 extrudePoint(glm::vec3 a, glm::vec3 b, double length) {
    glm::vec3 d = b - a;
    glm::vec3 d2 = d * d;
    double l = sqrt(d2.x + d2.y + d2.z);
    d = d * (float)(length / l);
    return a + d;
}