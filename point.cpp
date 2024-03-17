#include "point.h"

Point::Point(glm::vec3 pos) {
    defaultLateConstructor({ pos.x, pos.y, pos.z }, { 0.9,0.9,0.9 });
}