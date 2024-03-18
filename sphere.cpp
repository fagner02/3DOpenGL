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

Sphere::Sphere(double radius, int steps, glm::vec3 color) : VertexArray() {
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
                        glm::vec3 points[4] = { calcPoint(indexi, indexj - start, z_step, step, h, diameter, sign[k], sign[o], f),
                        calcPoint(indexi + 1, (indexj)-(start), z_step, step, h, diameter, sign[k], sign[o], f),
                        calcPoint(indexi + 1, (indexj + 1) - (start), z_step, step, h, diameter, sign[k], sign[o],f),
                        calcPoint(indexi, (indexj + 1) - start, z_step, step, h, diameter, sign[k], sign[o],f) };

                        glm::vec3 normal = glm::cross((points[0] - points[1]), (points[2] - points[1]));
                        for (int p = 0; p < 3;p++) {
                            for (int n = 0;n < 3;n++) {
                                vertices.push_back(points[p][n]);
                                colors.push_back(color[n]);
                                normals.push_back(normal[n]);
                            }
                        }

                        normal = glm::cross((points[1] - points[2]), (points[3] - points[2]));

                        int indexs[3] = { 0,2,3 };
                        for (int p = 0; p < 3;p++) {
                            for (int n = 0;n < 3;n++) {
                                vertices.push_back(points[indexs[p]][n]);
                                colors.push_back(color[n]);
                                normals.push_back(normal[n]);
                            }
                        }

                        indexj++;
                    }
                    glm::vec3 points[3] = { calcPoint(indexi, indexj - start, z_step, step, h, diameter, sign[k], sign[o], f),
                        calcPoint(indexi + 1, (indexj)-(start), z_step, step, h, diameter, sign[k], sign[o], f),
                        calcPoint(indexi + 1, (indexj + 1) - (start), z_step, step, h, diameter, sign[k], sign[o], f)
                    };

                    glm::vec3 normal = glm::cross((points[0] - points[1]), (points[2] - points[1]));
                    for (int p = 0; p < 3;p++) {
                        for (int n = 0;n < 3;n++) {
                            vertices.push_back(points[p][n]);
                            colors.push_back(color[n]);
                            normals.push_back(normal[n]);
                        }
                    }
                    indexj++;
                    indexi++;
                    start = indexj;
                    count += 2;
                }
            }
        }
    }

    defaultLateConstructor(vertices, colors, normals);
}