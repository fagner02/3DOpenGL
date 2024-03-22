#include "cube.h"

Cube::Cube(glm::vec3 pos, float size) {
    this->pos = pos;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indexes;
    struct point {
        int x, y;
    };

    size /= 2;
    point axis[] = { {1,2},{0,2},{0,1} };
    int sign[] = { -1,1 };
    for (int i = 0; i < 3; i++) {
        for (int k = 0;k < 2;k++) {
            point points[] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
            glm::vec3 s1[4];
            for (int j = 0; j < 4; j++) {
                glm::vec3 start(size);
                start[i] *= sign[k];
                start[axis[i].x] *= points[j].x;
                start[axis[i].y] *= points[j].y;
                s1[j] = start;
                start += pos;

                vertices.push_back(start);
            }
            glm::vec3 normal = glm::cross((s1[0] - s1[1]), (s1[2] - s1[1]));
            if (glm::dot(normal, pos - s1[0]) >= 0)
                normal *= -1;

            for (int j = 0; j < 4; j++) {
                normals.push_back(normal);

                colors.push_back({ 0.7,0.3,0.9 });
            }
            for (int j = 0; j < 3; j++) {
                indexes.push_back(j + i * 8 + k * 4);
            }
            for (int j = 1; j <= 3; j++) {
                indexes.push_back(j + i * 8 + k * 4);
            }
        }
    }

    initialize({ vertices, colors, normals, {}, indexes });
}