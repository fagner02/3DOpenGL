#include "shell_texture.h"

void ShellTexture::initialize(VertexArray *va, int shellNum, float shellHeight,
                              int divisions) {
    this->shellNum = shellNum;
    this->shellHeight = shellHeight;
    this->divisions = divisions;
    hasIndexes = va->originalBuffers.indexes.size() > 0;
    meshes.push_back(new VertexArray(va->originalBuffers));

    for (size_t i = 1; i < shellNum; i++) {
        meshes.push_back(new VertexArray(meshes[i - 1]->originalBuffers));
        for (size_t j = 0; j < va->originalBuffers.coordinates.size(); j++) {
            glm::vec3 newPoint =
                extrudePoint(glm::vec3(0.0), va->originalBuffers.normals[j],
                             ((float)i / shellNum) * shellHeight);
            meshes[i]->originalBuffers.coordinates[j] =
                va->originalBuffers.coordinates[j] + newPoint;
        }
    }
}
ShellTexture::ShellTexture(VertexArray *va, int shellNum, float shellHeight,
                           int divisions, glm::mat4 &modelMatrix,
                           std::vector<Mesh> modelMeshes)
    : modelMatrix(modelMatrix) {
    this->modelMeshes = modelMeshes;
    initialize(va, shellNum, shellHeight, divisions);
}

ShellTexture::ShellTexture(VertexArray *va, int shellNum, float shellHeight,
                           int divisions, glm::mat4 &modelMatrix)
    : modelMatrix(modelMatrix) {
    this->modelMeshes = std::vector<Mesh>();
    initialize(va, shellNum, shellHeight, divisions);
}
VertexArray *ShellTexture::getFirstShell() { return meshes[0]; }

void ShellTexture::applyMatrix(unsigned int shaderProgram) {
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void ShellTexture::draw(unsigned int shaderProgram) {
    int dloc = glGetUniformLocation(shaderProgram, "divisions");
    glUniform1f(dloc, divisions);
    int snloc = glGetUniformLocation(shaderProgram, "shellNum");
    glUniform1f(snloc, shellNum);
    int siloc = glGetUniformLocation(shaderProgram, "shellIndex");

    for (size_t i = 0; i < shellNum; i++) {
        glUniform1f(siloc, i);

        // applyMatrix(shaderProgram);
        meshes[i]->applyMatrix(shaderProgram);
        meshes[i]->bindVAO();
        if (modelMeshes.size() > 0) {
            for (int i = 0; i < modelMeshes.size(); i++) {
                glDrawElementsBaseVertex(
                    GL_TRIANGLES, modelMeshes[i].indexNum, GL_UNSIGNED_INT,
                    (void *)(sizeof(unsigned int) * modelMeshes[i].baseIndex),
                    modelMeshes[i].baseVertex);
            }
        } else if (hasIndexes) {
            glDrawElements(GL_TRIANGLES, meshes[i]->bufferCount,
                           GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, meshes[i]->bufferCount);
        }
    }
}