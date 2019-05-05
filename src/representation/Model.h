#ifndef SPACESHOOTER_MODEL_H
#define SPACESHOOTER_MODEL_H

#include "Mesh.h"
#include "Shader.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Model {
  vector<Mesh> meshes;
  vector<Texture> loadedTextures;
  string directory;

  void loadModel(const string &path);
  void processNode(const aiNode *node, const aiScene *scene);
  Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       TextureMapType typeName);

public:
  Model(const string &path) {
    loadModel(path);
  }

  void draw(Shader &shader) const;
};

#endif // SPACESHOOTER_MODEL_H
