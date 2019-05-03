#ifndef SPACESHOOTER_SCENE_H
#define SPACESHOOTER_SCENE_H

#include "../objects/Object.h"
#include "../objects/Player.h"
#include "../objects/Star.h"
#include "../representation/Skybox.h"
class Scene {
  Player *player;
  std::vector<Object *> objects;
  std::vector<Star *> stars;
  Skybox *skybox;

  Shader objShader = Shader("object/vertex.glsl", "object/fragment.glsl");
  Shader lightShader = Shader("light/vertex.glsl", "light/fragment.glsl");

public:
  Scene();
  ~Scene();

  void addObject(Object *obj);
  void draw(Camera &camera);
  void update(float deltaTime);

  Player *getPlayer();
};



#endif //SPACESHOOTER_SCENE_H
