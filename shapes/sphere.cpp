#include "sphere.h"

glm::vec3 Sphere::normalize(glm::vec3 a, glm::vec3 b, double length) {
    glm::vec3 d = b - a;
    glm::vec3 d2 = d * d;
    double l = sqrt(d2.x + d2.y + d2.z);
    d = d * (float)(length / l);
    return a + d;
}

glm::vec3 Sphere::calcPoint(int i, int j, double& z_step, double& step, double& h, double& diameter, int& sign1, int& sign2, bool flip) {
    double z = cos(glm::radians(45.0f)) * i * z_step;
    double y = h - (sin(glm::radians(45.0f)) * i * step);
    double x = -((step * i) / 2.0);
    x = ((x + (step * j)) * sign1);
    z = z * sign1;
    return normalize(glm::vec3(0.0f), glm::vec3(flip ? z : x, y * sign2, flip ? x : z), diameter);
}

Sphere::Sphere(double radius, glm::vec3 pos, int steps, glm::vec3 color) : VertexArray() {
    glm::vec3 center(0.0, 0.0, 0.0);
    double h = sqrt((radius * radius) * 2.0);
    double diameter = 2 * radius;
    double stepCount = steps;
    double step = diameter / stepCount;
    double z_step = h / stepCount;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;

    int sign[2] = { 1,-1 };

    for (int f = 0; f < 2; f++) {
        for (int o = 0; o < 2; o++) {
            for (int k = 0;k < 2;k++) {
                int count = 1;
                int indexj = 0;
                int indexi = 0;
                int start = 0;
                for (int i = 1; i <= stepCount; i++) {
                    for (int j = 0; j < (count / 2); j++) {
                        glm::vec3 points[4] = { calcPoint(indexi, indexj - start, z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj)-(start), z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj + 1) - (start), z_step, step, h, diameter, sign[k], sign[o],f) + pos,
                        calcPoint(indexi, (indexj + 1) - start, z_step, step, h, diameter, sign[k], sign[o],f) + pos };

                        glm::vec3 normal = glm::cross((points[1] - points[0]), (points[2] - points[0]));
                        if (glm::dot(normal, pos - points[1]) >= 0) {
                            normal *= -1;
                        }
                        for (int p = 0; p < 3;p++) {
                            vertices.push_back(points[p]);
                            colors.push_back(color);
                            normals.push_back(normal);

                        }

                        normal = (glm::cross((points[2] - points[0]), (points[3] - points[0])));
                        if (glm::dot(normal, pos - points[1]) >= 0) {
                            normal *= -1;
                        }
                        int indexs[3] = { 3,0, 2 };
                        for (int p = 0; p < 3;p++) {
                            vertices.push_back(points[indexs[p]]);
                            colors.push_back(color);
                            normals.push_back(normal);

                        }

                        indexj++;
                    }
                    glm::vec3 points[3] = { calcPoint(indexi, indexj - start, z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj)-(start), z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj + 1) - (start), z_step, step, h, diameter, sign[k], sign[o], f) + pos
                    };

                    glm::vec3 normal = (glm::cross((points[1] - points[0]), (points[2] - points[0])));
                    if (glm::dot(normal, pos - points[1]) >= 0) {
                        normal *= -1;
                    }
                    for (int p = 0; p < 3;p++) {
                        vertices.push_back(points[p]);
                        colors.push_back(color);
                        normals.push_back(normal);

                    }
                    indexj++;
                    indexi++;
                    start = indexj;
                    count += 2;
                }
            }
        }
    }

    initialize({ vertices, colors, normals });
}