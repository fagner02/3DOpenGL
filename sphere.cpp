#include "sphere.h"

glm::vec3 Sphere::normalize(glm::vec3 a, glm::vec3 b, double length) {
    glm::vec3 d = b - a;
    glm::vec3 d2 = d * d;
    double l = sqrt(d2.x + d2.y + d2.z);
    d = d * (float)(length / l);
    return a + d;
}

Sphere::Sphere (double radius, int steps) : VertexArray() {
    glm::vec3 center(0.0, 0.0, 0.0);
    double h = sqrt((radius * radius) * 2.0);
    double diameter = 2 * radius;
    double angle = glm::radians(45.0f);
    double stepCount = steps;
    double step = diameter / stepCount;
    double z_step = h / stepCount;

    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<float> normals;
    std::vector<unsigned int> indexes;

    double index = 0;
    for (int k = 0; k < 8; k++) {
        int count = 1;
        for (int i = 1; i <= stepCount;i++) {
            for (int j = 0; j < (count / 2);j++) {
                indexes.push_back(index);
                indexes.push_back(index + i);
                indexes.push_back(index + i + 1);
                indexes.push_back(index);
                indexes.push_back(index + 1);
                indexes.push_back(index + i + 1);
                index++;
            }
            indexes.push_back(index);
            indexes.push_back(index + i);
            indexes.push_back(index + i + 1);
            index++;

            count += 2;
        }
        index += stepCount + 1;
    }

    double v[2] = { 1,-1 };
    
    for (int o = 0; o < 2; o++) {
        for (int k = 0;k < 2;k++) {
            for (int i = 0; i <= stepCount; i++) {
                double z = cos(angle) * (double)i * z_step;
                double y = h - (sin(angle) * (double)i * step);
                double x = -((step*(double)i) / 2.0);
                for (int j = 0; j <= i; j++) {
                    glm::vec3 n = normalize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3((x + (step * (double)j))*v[k], y*v[o], z*v[k]), diameter);
                    vertices.push_back(n.x);
                    vertices.push_back(n.y);
                    vertices.push_back(n.z);
                    colors.push_back(1);
                    colors.push_back(1);
                    colors.push_back(1);
                    normals.push_back(0);
                    normals.push_back(0);
                    normals.push_back(0);
                }
            }
        }

        for (int k = 0;k < 2;k++) {
            for (int i = 0; i <= stepCount; i++) {
                double x = cos(angle) * (double)i * z_step;
                double y = h - (sin(angle) * (double)i * step);
                double z = -((step*(double)i) / 2.0);
                
                for (int j = 0; j <= i; j++) {
                    glm::vec3 n = normalize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(x * v[k], y*v[o], (z + (step * (double)j)) * v[k]), diameter);
                    vertices.push_back(n.x);
                    vertices.push_back(n.y);
                    vertices.push_back(n.z);
                    colors.push_back(1);
                    colors.push_back(1);
                    colors.push_back(1);
                    normals.push_back(0);
                    normals.push_back(0);
                    normals.push_back(0);
                }
            }
        }
    }

    indexLateConstructor(vertices, colors, normals, indexes);
}