#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <spdlog/spdlog.h>

#define CHECK_MESH(mesh)                                                                                               \
  if (mesh->mNormals == nullptr) {                                                                                     \
    throw std::runtime_error("Missing normals on the mesh");                                                           \
  } else if (mesh->mTextureCoords == nullptr) {                                                                        \
    throw std::runtime_error("Missing texture coordinates on the mesh");                                               \
  }

void Model::loadModel(const string &path) {
  SPDLOG_INFO("Loading model: {}", path);
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    throw std::runtime_error(std::string("Failed to load model: ").append(importer.GetErrorString()));
  }
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode *node, const aiScene *scene) {
  // process node mesh
  // TODO: create relations between meshes
  for (unsigned i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  // repeat process for all its children
  for (unsigned i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<unsigned> indices;
  vector<Texture> textures;

  // retrieve vertex information from mesh
  for (unsigned i = 0; i < mesh->mNumVertices; i++) {
    CHECK_MESH(mesh);
    Vertex vertex(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                  glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z), glm::vec2(0.f, 0.f));
    if (mesh->mTextureCoords[0]) {
      vertex.textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    vertices.push_back(vertex);
  }

  // process indices
  for (unsigned i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // process mesh material
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureMapType::Diffuse);
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureMapType::Specular);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }
  return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureMapType typeName) {
  vector<Texture> textures;
  unsigned textureCount = mat->GetTextureCount(type);

  for (unsigned i = 0; i < textureCount; i++) {
    // load texture
    // paths are expected to be local to the model
    aiString localPath;
    mat->GetTexture(type, i, &localPath);
    std::string path = directory + std::string("/").append(localPath.C_Str());
    SPDLOG_DEBUG("Searching for material: {}", path);

    // look for texture among already loaded ones
    bool isMatchFound = false;
    for (Texture &t : loadedTextures) {
      if (t.getPath() == path) {
        SPDLOG_INFO("Found match");
        textures.push_back(t);
        isMatchFound = true;
      }
    }

    // load texture if it was not loaded yet
    if (!isMatchFound) {
      Texture texture{typeName, path};
      loadedTextures.push_back(texture);
      textures.push_back(texture);
    }
  }
  return textures;
}

void Model::Draw(Shader &shader) const {
  for (const Mesh &mesh : meshes) {
    mesh.Draw(shader);
  }
}
