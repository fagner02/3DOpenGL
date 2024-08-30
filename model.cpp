#include "model.h"

Model::Model() {}

VAOBuffers Model::getModelBuffers(const char* name) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (scene == nullptr) {
        std::cout << "Couldn't load model: " << name << "\n";
    }
    meshes.resize(scene->mNumMeshes);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indexes;
    unsigned int vertex = 0;
    unsigned int index = 0;
    unsigned int maxIndex = 0;
    unsigned int indexCount = 0;

    for (int i = 0;i < scene->mNumMeshes;i++) {
        meshes[i].baseVertex = vertex;
        meshes[i].baseIndex = index;
        meshes[i].indexNum = scene->mMeshes[i]->mNumFaces * 3;
        meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        vertex += scene->mMeshes[i]->mNumVertices;
        index += scene->mMeshes[i]->mNumFaces * 3;
        aiVector3D* mVertices = scene->mMeshes[i]->mVertices;
        aiVector3D* mNormals = scene->mMeshes[i]->mNormals;
        aiVector3D** textureCoords = scene->mMeshes[i]->mTextureCoords;
        int uvIndex = -1;
        for (size_t k = 0; k < 8; k++) {
            if (scene->mMeshes[i]->HasTextureCoords(k)) {
                uvIndex = k;
                break;
            }
        }

        for (int j = 0;j < scene->mMeshes[i]->mNumVertices; j++) {
            vertices.push_back(glm::vec3(mVertices[j].x, mVertices[j].y, mVertices[j].z) * 0.001f);

            aiColor3D color(0.f, 0.f, 0.f);
            scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            normals.push_back(glm::vec3(mNormals[j].x, mNormals[j].y, mNormals[j].z));
            if (uvIndex > -1)
                uvs.push_back(glm::vec2(textureCoords[uvIndex][j].x, textureCoords[uvIndex][j].y));
            colors.push_back(glm::vec3(color.r, color.g, color.b));
        }

        for (int j = 0;j < scene->mMeshes[i]->mNumFaces; j++) {
            assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
            for (int k = 0;k < 3; k++) {
                indexes.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);

                indexCount++;
                if (indexes.back() + index > maxIndex) {
                    maxIndex = indexes.back() + index - meshes[i].indexNum;
                }
            }
        }
    }

    return { vertices, colors, normals, uvs, indexes };
}

void Model::loadFile(const char* name) {
    initialize(getModelBuffers(name));
    filename = std::string(name);
    // loadTextures(scene);
    // delete scene;
}

void Model::saveFile(std::string destfile, glm::mat4 transform) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (scene == nullptr) {
        std::cout << "Couldn't load model: " << filename << "\n";
    }

    for (int i = 0;i < scene->mNumMeshes;i++) {
        for (int j = 0;j < scene->mMeshes[i]->mNumVertices; j++) {
            glm::vec4 res = transform * glm::vec4(
                scene->mMeshes[i]->mVertices[j].x,
                scene->mMeshes[i]->mVertices[j].y,
                scene->mMeshes[i]->mVertices[j].z,
                1
            );
            scene->mMeshes[i]->mVertices[j].x = res.x;
            scene->mMeshes[i]->mVertices[j].y = res.y;
            scene->mMeshes[i]->mVertices[j].z = res.z;
        }
    }

    Assimp::Exporter exporter;
    std::string format = destfile.substr(destfile.find_last_of(".") + 1);
    aiReturn res = exporter.Export(scene, format, destfile);
}

void Model::loadTextures(const aiScene* scene) {
    for (int i = 0; i < scene->mNumMaterials;i++) {
        if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string filename(path.C_Str());
                int last = filename.find_last_of('/');
                filename = "./textures/" + filename.substr(last);

                textures.push_back(Texture(filename.c_str()));
            }
        } else {
            std::cout << "Couldn't get texture\n";
        }
    }
}

void Model::draw(int shaderProgram) {
    applyMatrix(shaderProgram);
    bindVAO();
    for (int i = 0;i < meshes.size();i++) {
        // textures[meshes[i].materialIndex].bindTexture();
        glDrawElementsBaseVertex(GL_TRIANGLES, meshes[i].indexNum, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * meshes[i].baseIndex), meshes[i].baseVertex);
    }
}