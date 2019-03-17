//
// Created by Max Zhuravsky on 2019-03-17.
//

#include "Model.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::loadModel(const string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(std::string("Failed to load model: ").append(importer.GetErrorString()));
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode *node, const aiScene *scene) {
    // process node mesh
    // TODO: create relations between meshes
    for(unsigned i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        std::cout << "[MODEL] Current mesh count: " << meshes.size() << std::endl;
    }

    // repeat process for all its children
    for(unsigned i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned> indices;
    vector<Texture> textures;

    // retrieve vertex information from mesh
    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex(
                glm::vec3(
                        mesh->mVertices[i].x,
                        mesh->mVertices[i].y,
                        mesh->mVertices[i].z
                ),
                glm::vec3(
                        mesh->mNormals[i].x,
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z
                ),
                glm::vec2(0.f, 0.f)
        );
        if (mesh->mTextureCoords[0]) {
            vertex.textureCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        vertices.push_back(vertex);
    }

    // process indices
    for(unsigned i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process mesh material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string &typeName) {
    vector<Texture> textures;
    unsigned textureCount = mat->GetTextureCount(type);
    for (unsigned i = 0; i < textureCount; i++) {
        // load texture
        // paths are expected to be local to the model
        aiString localPath;
        mat->GetTexture(type, i, &localPath);
        std::string path = directory + std::string("/").append(localPath.C_Str());

        // look for texture among already loaded ones
        for (Texture &t : loadedTextures) {
            if (t.getPath() == path) {
                textures.push_back(t);
                break;
            }
        }

        // load texture if it was not loaded yet
        Texture texture{
                typeName,
                path
        };
        loadedTextures.push_back(texture);
        textures.push_back(texture);
    }
    return textures;
}

void Model::Draw(Shader &shader) {
    for (const Mesh &mesh : meshes) {
        mesh.Draw(shader);
    }
}
