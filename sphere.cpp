#include "sphere.h"

glm::vec3 Sphere::normalize(glm::vec3 a, glm::vec3 b, double length) {
    glm::vec3 d = b - a;
    glm::vec3 d2 = d * d;
    double l = sqrt(d2.x + d2.y + d2.z);
    d = d * (float)(length / l);
    return a + d;
}

glm::vec3 Sphere::calcPoint(int i, int j, double& z_step, double& step, double& h, double& diameter, int& sign1, int& sign2) {
    double z = cos(glm::radians(45.0f)) * i * z_step;
    double y = h - (sin(glm::radians(45.0f)) * i * step);
    double x = -((step * i) / 2.0);

    return normalize(glm::vec3(0.0f), glm::vec3(((x + (step * j)) * sign1), y * sign2, z * sign1), diameter);
}

Sphere::Sphere(double radius, int steps) : VertexArray() {
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

    for (int o = 0; o < 2; o++) {

        for (int k = 0;k < 2;k++) {
            int count = 1;
            int indexj = 0;
            int indexi = 0;
            int start = 0;
            for (int i = 1; i <= stepCount; i++) {
                // double z = cos(angle) * (double)i * z_step;
                // double y = h - (sin(angle) * (double)i * step);
                // double x = -((step * (double)i) / 2.0);

                for (int j = 0; j < (count / 2); j++) {
                    std::cout << indexi << " " << indexj - indexi << " \n";
                    glm::vec3 points[4] = { calcPoint(indexi, indexj - indexi, z_step, step, h, diameter, sign[k], sign[o]),
                    calcPoint(indexi + 1, (indexj + i) - (indexi + 1), z_step, step, h, diameter, sign[k], sign[o]),
                    calcPoint(indexi + 1, (indexj + i + 1) - (indexi + 1), z_step, step, h, diameter, sign[k], sign[o]),
                    calcPoint(indexi, (indexj + 1) - indexi, z_step, step, h, diameter, sign[k], sign[o]) };

                    glm::vec3 normal = glm::cross((points[0] - points[1]), (points[2] - points[1]));
                    for (int p = 0; p < 3;p++) {
                        for (int n = 0;n < 3;n++) {
                            vertices.push_back(points[p][n]);
                            colors.push_back(1);
                            normals.push_back(normal[n]);
                        }
                    }

                    normal = glm::cross((points[1] - points[2]), (points[3] - points[2]));
                    for (int p = 1; p <= 3;p++) {
                        for (int n = 0;n < 3;n++) {
                            vertices.push_back(points[p][n]);
                            colors.push_back(1);
                            normals.push_back(normal[n]);
                        }
                    }

                    indexj++;
                    // int j1 = index - count;
                    // glm::vec3 n = normalize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3((x + (step * (double)j)) * sign[k], y * sign[o], z * sign[k]), diameter);
                    // vertices.push_back(n.x);
                    // vertices.push_back(n.y);
                    // vertices.push_back(n.z);
                    // colors.push_back(1);
                    // colors.push_back(1);
                    // colors.push_back(1);
                }
                glm::vec3 points[3] = { calcPoint(indexi, indexj - indexi, z_step, step, h, diameter, sign[k], sign[o]),
                    calcPoint(indexi + 1, (indexj + i) - (indexi + 1), z_step, step, h, diameter, sign[k], sign[o]),
                    calcPoint(indexi + 1, (indexj + i + 1) - (indexi + 1), z_step, step, h, diameter, sign[k], sign[o])
                };
                glm::vec3 normal = glm::cross((points[0] - points[1]), (points[2] - points[1]));
                for (int p = 0; p < 3;p++) {
                    for (int n = 0;n < 3;n++) {
                        vertices.push_back(points[p][n]);
                        colors.push_back(1);
                        normals.push_back(normal[n]);
                    }
                }
                indexj++;
                indexi++;
                start = indexj;
                count += 2;
            }
        }

        // for (int k = 0;k < 2;k++) {
        //     for (int i = 0; i <= stepCount; i++) {
        //         double x = cos(angle) * (double)i * z_step;
        //         double y = h - (sin(angle) * (double)i * step);
        //         double z = -((step * (double)i) / 2.0);

        //         for (int j = 0; j <= i; j++) {
        //             glm::vec3 n = normalize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(x * sign[k], y * sign[o], (z + (step * (double)j)) * sign[k]), diameter);
        //             vertices.push_back(n.x);
        //             vertices.push_back(n.y);
        //             vertices.push_back(n.z);
        //             colors.push_back(1);
        //             colors.push_back(1);
        //             colors.push_back(1);
        //         }
        //     }
        // }
    }

    // double index = 0;
    // for (int k = 0; k < 8; k++) {
    //     int count = 1;
    //     for (int i = 1; i <= stepCount;i++) {
    //         for (int j = 0; j < (count / 2);j++) {
    //             indexes.push_back(index);
    //             indexes.push_back(index + i);
    //             indexes.push_back(index + i + 1);
    //             glm::vec3 a(vertices[(index * 3)], vertices[(index * 3) + 1], vertices[(index * 3) + 2]);
    //             glm::vec3 b(vertices[((index + i) * 3)], vertices[((index + i) * 3) + 1], vertices[((index + i) * 3) + 2]);
    //             glm::vec3 c(vertices[((index + i + 1) * 3)], vertices[((index + i + 1) * 3) + 1], vertices[((index + i + 1) * 3) + 2]);
    //             glm::vec3 normal = glm::cross((a - b), (c - b));
    //             for (int o = 0;o < 3;o++) {
    //                 normals.push_back(normal[o]);
    //             }
    //             indexes.push_back(index);
    //             indexes.push_back(index + 1);
    //             indexes.push_back(index + i + 1);

    //             glm::vec3 d(vertices[((index + 1) * 3)], vertices[((index + 1) * 3) + 1], vertices[((index + 1) * 3) + 2]);
    //             normal = glm::cross((a - d), (c - d));
    //             for (int o = 0;o < 3;o++) {
    //                 normals.push_back(normal[o]);
    //             }
    //             index++;
    //         }
    //         indexes.push_back(index);
    //         indexes.push_back(index + i);
    //         indexes.push_back(index + i + 1);
    //         glm::vec3 a(vertices[(index * 3)], vertices[(index * 3) + 1], vertices[(index * 3) + 2]);
    //         glm::vec3 b(vertices[((index + i) * 3)], vertices[((index + i) * 3) + 1], vertices[((index + i) * 3) + 2]);
    //         glm::vec3 c(vertices[((index + i + 1) * 3)], vertices[((index + i + 1) * 3) + 1], vertices[((index + i + 1) * 3) + 2]);
    //         glm::vec3 normal = glm::cross((a - b), (c - b));
    //         for (int o = 0;o < 3;o++) {
    //             normals.push_back(normal[o]);
    //         }
    //         index++;

    //         count += 2;
    //     }
    //     index += stepCount + 1;
    // }

    defaultLateConstructor(vertices, colors, normals);
}