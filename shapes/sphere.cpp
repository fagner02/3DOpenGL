#include "sphere.h"

glm::vec3 Sphere::calcPoint(int i, int j, double& z_step, double& step, double& h, double& diameter, int& sign1, int& sign2, bool flip) {
    double z = cos(glm::radians(45.0f)) * i * z_step;
    double y = h - (sin(glm::radians(45.0f)) * i * step);
    double x = -((step * i) / 2.0);
    x = ((x + (step * j)) * sign1);
    z = z * sign1;
    return extrudePoint(glm::vec3(0.0f), glm::vec3(flip ? z : x, y * sign2, flip ? x : z), diameter);
}

Sphere::Sphere(double radius, glm::vec3 pos, int steps, glm::vec3 color) : VertexArray() {
    this->pos = pos;

    initialize(calcSphereBuffers(radius, pos, steps, color));
}

VAOBuffers Sphere::calcSphereBuffers(double radius, glm::vec3 pos, int steps, glm::vec3 color) {
    glm::vec3 center(0.0, 0.0, 0.0);

    double h = sqrt((radius * radius) * 2.0);
    double diameter = 2 * radius;
    double stepCount = steps;
    double step = diameter / stepCount;
    double z_step = h / stepCount;
    int vnum = 8 * ((3 * stepCount) * (stepCount));
    int vindex = 0;
    std::cout << stepCount << " " << vnum << " ";
    std::vector<glm::vec3> vertices(vnum, glm::vec3(0.0));
    std::vector<glm::vec3> colors(vnum, glm::vec3(0.0));
    std::vector<glm::vec3> normals(vnum, glm::vec3(0.0));
    std::vector<glm::vec2> uvs(vnum, glm::vec2(0.0));
    int sign[2] = { 1,-1 };

    for (int f = 0; f < 2; f++) {
        for (int o = 0; o < 2; o++) {
            for (int k = 0;k < 2;k++) {
                int count = 1;
                int indexj = 0;
                int indexi = 0;
                int start = 0;
                for (int i = 1; i <= stepCount; i++) {
                    double countTotal = (count / 2) + 1;
                    for (int j = 0; j < (count / 2); j++) {
                        std::vector<glm::vec2> uv = {
                            glm::vec2((float)indexi / stepCount, (float)(indexj - start) / countTotal),
                            glm::vec2((float)(indexi + 1) / stepCount,  (float)(indexj - start) / countTotal),
                            glm::vec2((float)(indexi + 1) / stepCount,  (float)(indexj + 1 - start) / countTotal),
                            glm::vec2((float)indexi / stepCount,  (float)(indexj + 1 - start) / countTotal)
                        };
                        glm::vec3 points[4] = { calcPoint(indexi, indexj - start, z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj)-(start), z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj + 1) - (start), z_step, step, h, diameter, sign[k], sign[o],f) + pos,
                        calcPoint(indexi, (indexj + 1) - start, z_step, step, h, diameter, sign[k], sign[o],f) + pos };

                        glm::vec3 normal = glm::cross((points[1] - points[0]), (points[2] - points[0]));
                        if (glm::dot(normal, pos - points[1]) >= 0) {
                            normal *= -1;
                        }
                        for (int p = 0; p < 3;p++) {
                            vertices[vindex] = points[p];
                            colors[vindex] = color;
                            normals[vindex] = normal;
                            uvs[vindex] = uv[p];
                            vindex++;
                        }

                        normal = (glm::cross((points[2] - points[0]), (points[3] - points[0])));
                        if (glm::dot(normal, pos - points[1]) >= 0) {
                            normal *= -1;
                        }
                        int indexs[3] = { 3, 0, 2 };
                        for (int p = 0; p < 3;p++) {
                            vertices[vindex] = points[indexs[p]];
                            colors[vindex] = color;
                            normals[vindex] = normal;
                            uvs[vindex] = uv[indexs[p]];
                            vindex++;
                        }

                        indexj++;
                    }
                    std::vector<glm::vec2> uv = {
                        glm::vec2((float)indexi / stepCount, (float)(indexj - start) / countTotal),
                        glm::vec2((float)(indexi + 1) / stepCount,  (float)(indexj - start) / countTotal),
                        glm::vec2((float)(indexi + 1) / stepCount,  (float)(indexj + 1 - start) / countTotal),
                    };


                    glm::vec3 points[3] = {
                        calcPoint(indexi, indexj - start, z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj)-(start), z_step, step, h, diameter, sign[k], sign[o], f) + pos,
                        calcPoint(indexi + 1, (indexj + 1) - (start), z_step, step, h, diameter, sign[k], sign[o], f) + pos
                    };

                    glm::vec3 normal = (glm::cross((points[1] - points[0]), (points[2] - points[0])));
                    if (glm::dot(normal, pos - points[1]) >= 0) {
                        normal *= -1;
                    }
                    for (int p = 0; p < 3;p++) {
                        vertices[vindex] = points[p];
                        colors[vindex] = color;
                        normals[vindex] = normal;
                        uvs[vindex] = uv[p];
                        vindex++;
                    }
                    indexj++;
                    indexi++;
                    start = indexj;
                    count += 2;
                }
            }
        }
    }
    std::cout << vertices.size();

    return { vertices, colors, normals, uvs };
}

void Sphere::draw(int shaderProgram) {
    bindVAO();
    applyMatrix(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, bufferCount);
}