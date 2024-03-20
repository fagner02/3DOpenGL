#include "model.h"

Model::Model() {}

Model::~Model() {}

void Model::loadFile(const char* name) {
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate);

    meshes.resize(scene->mNumMeshes);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indexes;
    std::vector<glm::vec2> textureCoord;
    unsigned int vertex = 0;
    unsigned int index = 0;
    for (int i = 0;i < scene->mNumMeshes;i++) {
        meshes[i].baseVertex = vertex;
        meshes[i].baseIndex = index;
        meshes[i].indexNum = scene->mMeshes[i]->mNumFaces;
        meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        vertex += scene->mMeshes[i]->mNumVertices;
        index += scene->mMeshes[i]->mNumFaces;
        aiVector3D* mVertices = scene->mMeshes[i]->mVertices;
        aiVector3D* mNormals = scene->mMeshes[i]->mNormals;
        aiVector3D** textureCoords = scene->mMeshes[i]->mTextureCoords;
        for (int j = 0;j < scene->mMeshes[i]->mNumVertices; j++) {
            vertices.push_back(glm::vec3(mVertices[j].x, mVertices[j].y, mVertices[j].z));
            normals.push_back(glm::vec3(mNormals[j].x, mNormals[j].y, mNormals[j].z));
            textureCoord.push_back(
                scene->mMeshes[i]->HasTextureCoords(0) ?
                glm::vec2(textureCoords[0][j].x, textureCoords[0][j].y) : glm::vec2(0));
        }
        for (int j = 0;j < scene->mMeshes[i]->mNumFaces; j++) {

            for (int k = 0;k < 3; k++) {
                indexes.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
            }
        }
    }

    initialize({ vertices, {}, normals, textureCoord, indexes });

    loadTextures(scene);
}

void Model::CreateLightList() {

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

void Model::draw() {
    vertexArray.bindVAO();
    for (int i = 0;i < meshes.size();i++) {
        textures[meshes[i].materialIndex].bindTexture();
        glDrawElementsBaseVertex(GL_TRIANGLES, meshes[i].indexNum, GL_UNSIGNED_INT, (void*)(meshes[i].baseIndex * 3), meshes[i].baseVertex);
    }
}

void Model::EnableLights() {
}

void Model::DisableLights() {

}