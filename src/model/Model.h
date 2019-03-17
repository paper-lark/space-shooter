#ifndef SPACESHOOTER_MODEL_H
#define SPACESHOOTER_MODEL_H

#include <vector>
#include <assimp/scene.h>
#include <string>
#include "Mesh.h"
#include "../utils/Shader.h"

using std::vector;
using std::string;

class Model {
    vector<Mesh> meshes;
    vector<Texture> loadedTextures;
    string directory;

    void loadModel(const string &path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string &typeName);

public:
    Model(const string &path) {
        loadModel(path);
    }

    void Draw(Shader &shader);
};


#endif //SPACESHOOTER_MODEL_H
