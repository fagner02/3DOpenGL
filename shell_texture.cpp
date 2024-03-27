#include "shell_texture.h"

ShellTexture::ShellTexture(VAOBuffers buffers, int shellNum, float shellHeight, int divisions, std::vector<Mesh> modelMeshes) {
    this->modelMeshes = modelMeshes;
    this->shellNum = shellNum;
    this->shellHeight = shellHeight;
    this->divisions = divisions;
    hasIndexes = buffers.indexes.size() > 0;
    std::vector<glm::vec3> vertices = buffers.coordinates;
    meshes.push_back(new VertexArray(buffers));
    for (size_t i = 1; i < shellNum; i++) {
        for (size_t j = 0; j < buffers.coordinates.size(); j++) {
            glm::vec3 newPoint = extrudePoint(glm::vec3(0.0), buffers.normals[j], ((float)i / shellNum) * shellHeight);
            buffers.coordinates[j] = buffers.coordinates[j] + newPoint;
        }
        meshes.push_back(new VertexArray(buffers));
        buffers.coordinates = vertices;
    }
}

void ShellTexture::draw(int shaderProgram) {
    int dloc = glGetUniformLocation(shaderProgram, "divisions");
    glUniform1f(dloc, divisions);
    int snloc = glGetUniformLocation(shaderProgram, "shellNum");
    glUniform1f(snloc, shellNum);
    int siloc = glGetUniformLocation(shaderProgram, "shellIndex");

    for (size_t i = 0; i < shellNum; i++) {
        glUniform1f(siloc, i);

        meshes[i]->applyMatrix(shaderProgram);
        meshes[i]->bindVAO();
        if (modelMeshes.size() > 0) {
            for (int i = 0;i < modelMeshes.size();i++) {
                glDrawElementsBaseVertex(GL_TRIANGLES, modelMeshes[i].indexNum, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * modelMeshes[i].baseIndex), modelMeshes[i].baseVertex);
            }
        } else if (hasIndexes) {
            glDrawElements(GL_TRIANGLES, meshes[i]->bufferCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, meshes[i]->bufferCount);
        }


    }
}