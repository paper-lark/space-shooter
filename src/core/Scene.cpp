#include "Scene.h"
#include "../objects/Spaceship.h"
#include "Application.h"

Scene::Scene() {
  player = new Player{1000, glm::vec3(0.f, 0.f, 0.f)};
  Application::getSingleton().bindPlayer(player);
  objects = {
      new Spaceship{100, glm::vec3(10.f, 6.f, -15.f)},  new Spaceship{100, glm::vec3(23.f, 11.f, 3.f)},
      new Spaceship{100, glm::vec3(1.f, -12.f, -13.f)}, new Spaceship{1000, glm::vec3(12.f, 5.f, -1.f)},
      new Spaceship{10000, glm::vec3(15.f, 2.f, 22.f)}, new Spaceship{10000, glm::vec3(-5.f, -12.f, -32.f)},
  };
  stars = {
      new Star{1000000, glm::vec3(105.7f, 13.75f, 19.0f)},
      new Star{1000000, glm::vec3(123.3f, -50.f, -55.0f)},
      new Star{1000000, glm::vec3(-35.0f, 76.0f, 83.0f)},
      new Star{1000000, glm::vec3(-58.25f, 33.75f, -143.0f)},
  };
  std::vector<std::string> textures_faces = {"assets/Skybox/lightblue/right.png", "assets/Skybox/lightblue/left.png",
                                             "assets/Skybox/lightblue/top.png",   "assets/Skybox/lightblue/bot.png",
                                             "assets/Skybox/lightblue/front.png", "assets/Skybox/lightblue/back.png"};
  skybox = new Skybox{textures_faces, Shader("skybox/vertex.glsl", "skybox/fragment.glsl")};
}

Scene::~Scene() {
  Application::getSingleton().bindPlayer(nullptr);
  delete skybox;
  delete player;
  std::for_each(stars.begin(), stars.end(), [](Star *star) { delete star; });
  std::for_each(objects.begin(), objects.end(), [](Object *obj) { delete obj; });
}

void Scene::draw(Camera &camera) {
  Light lightSpecs = Star::getLight();

  // draw object
  objShader.use();
  objShader.setMatrix("view", camera.getViewMatrix());
  objShader.setMatrix("projection", camera.getProjectionMatrix());
  objShader.setVec3("viewPos", camera.getPos());

  objShader.setVec3("dirLight.direction", glm::vec3(0.f, 10.f, 5.f));
  objShader.setVec3("dirLight.diffuse", lightSpecs.diffuse);
  objShader.setVec3("dirLight.ambient", lightSpecs.ambient);
  objShader.setVec3("dirLight.specular", lightSpecs.specular);

  objShader.setVec3("flashlight.position", camera.getPos());
  objShader.setVec3("flashlight.direction", QuatHelpers::getForward(camera.getOrientation()));
  objShader.setFloat("flashlight.innerCutOff", glm::cos(glm::radians(12.5f)));
  objShader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(17.5f)));
  objShader.setVec3("flashlight.diffuse", lightSpecs.diffuse);
  objShader.setVec3("flashlight.ambient", lightSpecs.ambient);
  objShader.setVec3("flashlight.specular", lightSpecs.specular);
  objShader.setFloat("flashlight.constant", lightSpecs.constant);
  objShader.setFloat("flashlight.linear", lightSpecs.linear);
  objShader.setFloat("flashlight.quadratic", lightSpecs.quadratic);

  for (int i = 0; i < stars.size(); i++) {
    std::string prefix = "pointLights[" + std::to_string(i) + "]";
    objShader.setVec3(prefix + ".position", stars[i]->getPosition());
    objShader.setVec3(prefix + ".diffuse", lightSpecs.diffuse);
    objShader.setVec3(prefix + ".ambient", lightSpecs.ambient);
    objShader.setVec3(prefix + ".specular", lightSpecs.specular);
    objShader.setFloat(prefix + ".constant", lightSpecs.constant);
    objShader.setFloat(prefix + ".linear", lightSpecs.linear);
    objShader.setFloat(prefix + ".quadratic", lightSpecs.quadratic);
  }

  for (Object *ship : objects) {
    ship->draw(objShader);
  }
  player->draw(objShader);

  // draw light sources
  lightShader.use();
  lightShader.setMatrix("view", camera.getViewMatrix());
  lightShader.setMatrix("projection", camera.getProjectionMatrix());
  for (Star *star : stars) {
    lightShader.setMatrix("model", star->getObjectModelMatrix());
    star->draw(lightShader);
  }

  // draw skybox
  skybox->draw(camera);
}

bool isBoundingBoxColliding(glm::vec3 posA, glm::vec3 dimsA, glm::vec3 posB, glm::vec3 dimsB) {
  // TODO: implement
  return false;
}

void Scene::update(float deltaTime) {
  player->update(deltaTime);
  for (Object *ship : objects) {
    ship->update(deltaTime);
  }
  for (Star *star : stars) {
    star->update(deltaTime);
  }

  // check collisions
  // TODO: check with stars and player too
  for (Object *obj : objects) {
    for (Object *other : objects) {
      if (isBoundingBoxColliding(std::get<0>(obj->getBBox()), std::get<1>(obj->getBBox()),
                                 std::get<0>(other->getBBox()), std::get<1>(obj->getBBox()))) {
        unsigned damage = std::min(obj->getHealth(), other->getHealth());
        obj->applyDamage(damage);
        other->applyDamage(damage);
      }
    }
  }

  // remove all dead objects
  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [](const Object *s) {
                                 if (!s->isAlive()) {
                                   delete s;
                                   return true;
                                 }
                                 return false;
                               }),
                objects.end());
}

const Player *Scene::getPlayer() {
  return player;
}