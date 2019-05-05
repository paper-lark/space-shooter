#include "Scene.h"
#include "../configuration.h"
#include "../objects/Asteroid.h"
#include "../objects/Spaceship.h"
#include "../objects/Torpedo.h"
#include "Application.h"
#include <spdlog/spdlog.h>

#define MAX_POINT_LIGHT_COUNT 4
#define FLASHLIGHT_INTENSITY 0.33f
#define DIRECTIONAL_LIGHT_INTENSITY 0.1f
#define DIRECTIONAL_LIGHT_DIR glm::vec3(-6.f, 15.f, 7.5f)
#define POINT_LIGHT_COLOR 15.f

#define BORDER_OFFSET 10.f
#define OBJECT_CREATION_TIMEOUT 3.f
#define STAR_CREATION_TIMEOUT 90.f
#define TORPEDO_DAMAGE 100.f

Scene::Scene() {
  // create tracks
  for (int i = 0; i < TRACKS_PER_DIRECTION; i++) {
    for (int j = 0; j < TRACKS_PER_DIRECTION; j++) {
      tracks[i * TRACKS_PER_DIRECTION + j] =
          Track{glm::vec3(i * TRACK_SIZE, j * TRACK_SIZE, -SPAWN_FIELD_DEPTH),
                glm::vec3(i * TRACK_SIZE, j * TRACK_SIZE, SPAWN_FIELD_DEPTH)};
    }
  }

  // create player
  float planeLimit = TRACKS_PER_DIRECTION * TRACK_SIZE;
  player = new Player{1000, glm::vec3(planeLimit / 2, planeLimit / 2, 0.f), 180};

  // create skybox
  std::vector<std::string> textures_faces = {
      "assets/Skybox/right.png", "assets/Skybox/left.png",
      "assets/Skybox/top.png",   "assets/Skybox/bot.png",
      "assets/Skybox/front.png", "assets/Skybox/back.png"};
  skybox = new Skybox{textures_faces, Shader("skybox/vertex.glsl", "skybox/fragment.glsl")};
}

Scene::~Scene() {
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

  objShader.setVec3("dirLight.direction", DIRECTIONAL_LIGHT_DIR);
  objShader.setVec3("dirLight.diffuse", DIRECTIONAL_LIGHT_INTENSITY * lightSpecs.diffuse);
  objShader.setVec3("dirLight.ambient", DIRECTIONAL_LIGHT_INTENSITY * lightSpecs.ambient);
  objShader.setVec3("dirLight.specular", DIRECTIONAL_LIGHT_INTENSITY * lightSpecs.specular);

  objShader.setVec3("flashlight.position", camera.getPos());
  objShader.setVec3("flashlight.direction", QuatHelpers::getForward(camera.getOrientation()));
  objShader.setFloat("flashlight.innerCutOff", glm::cos(glm::radians(12.5f)));
  objShader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(17.5f)));
  objShader.setVec3("flashlight.diffuse", FLASHLIGHT_INTENSITY * lightSpecs.diffuse);
  objShader.setVec3("flashlight.ambient", FLASHLIGHT_INTENSITY * lightSpecs.ambient);
  objShader.setVec3("flashlight.specular", FLASHLIGHT_INTENSITY * lightSpecs.specular);
  objShader.setFloat("flashlight.constant", lightSpecs.constant);
  objShader.setFloat("flashlight.linear", lightSpecs.linear);
  objShader.setFloat("flashlight.quadratic", lightSpecs.quadratic);

  for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {
    std::string prefix = "pointLights[" + std::to_string(i) + "]";
    objShader.setVec3(prefix + ".position", glm::vec3(0.f, 0.f, 0.f));
  }

  for (int i = 0; i < stars.size() && i < MAX_POINT_LIGHT_COUNT; i++) {
    std::string prefix = "pointLights[" + std::to_string(i) + "]";
    objShader.setVec3(prefix + ".position", stars[i]->getPosition());
    objShader.setVec3(prefix + ".diffuse", POINT_LIGHT_COLOR * lightSpecs.diffuse);
    objShader.setVec3(prefix + ".ambient", POINT_LIGHT_COLOR * lightSpecs.ambient);
    objShader.setVec3(prefix + ".specular", POINT_LIGHT_COLOR * lightSpecs.specular);
    objShader.setFloat(prefix + ".constant", lightSpecs.constant);
    objShader.setFloat(prefix + ".linear", lightSpecs.linear);
    objShader.setFloat(prefix + ".quadratic", lightSpecs.quadratic);
  }

  for (Track &track : tracks) {
    track.draw(objShader);
  }
  for (Object *obj : objects) {
    obj->draw(objShader);
  }
  player->draw(objShader);

  // draw stars
  simpleShader.use();
  simpleShader.setMatrix("view", camera.getViewMatrix());
  simpleShader.setMatrix("projection", camera.getProjectionMatrix());
  for (Star *star : stars) {
    star->draw(simpleShader);
  }

  // draw particles
  particles.draw(simpleShader);

  // draw skybox
  skybox->draw(camera);
}

bool isBoundingBoxColliding(glm::vec4 bboxA, glm::vec4 bboxB) {
  glm::vec3 sizeA{bboxA};
  glm::vec3 sizeB{bboxB};
  return glm::length(sizeA - sizeB) < bboxA.w + bboxB.w;
}

void resolveCollision(Object *first, Object *second) {
  glm::vec4 bboxA = first->getBBox();
  glm::vec4 bboxB = second->getBBox();

  if (isBoundingBoxColliding(bboxA, bboxB)) {
    SPDLOG_INFO("Collision detected: {} {} {} and {} {} {}", bboxA.x, bboxA.y, bboxA.z, bboxB.x,
                bboxB.y, bboxB.z);
    unsigned damage = std::min(first->getHealth(), second->getHealth());
    SPDLOG_INFO("Inflicting {} pts of damage", damage);
    first->applyDamage(damage);
    second->applyDamage(damage);
  }
}

unsigned Scene::update(float deltaTime) {
  // update all scene objects
  player->update(deltaTime);
  std::for_each(tracks.begin(), tracks.end(),
                [deltaTime](Track &track) { track.update(deltaTime); });
  std::for_each(objects.begin(), objects.end(),
                [deltaTime](Object *obj) { obj->update(deltaTime); });
  std::for_each(stars.begin(), stars.end(), [deltaTime](Object *obj) { obj->update(deltaTime); });

  // update particle system centered on the player
  particles.setPosition(player->getPosition() + WARP_EFFECT_OFFSET);
  particles.update(deltaTime);

  // check borders
  this->checkBorders();
  std::for_each(tracks.begin(), tracks.end(), [](Track &track) { track.checkBorders(); });

  // spawn new objects
  this->addNewObjects(deltaTime);

  // check collisions
  return this->processCollisions();
}

Player *Scene::getPlayer() {
  return player;
}

void Scene::addObjectOnTrack(Object *obj, unsigned trackNumber) {
  tracks.at(trackNumber).add(obj);
}

void Scene::addObject(Object *obj) {
  objects.push_back(obj);
}

void Scene::checkBorders() {
  // filter auxiliary objects
  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [](const Object *obj) {
                                 glm::vec3 objPosition = obj->getPosition();
                                 float planeLimit = TRACKS_PER_DIRECTION * TRACK_SIZE;
                                 bool shouldRemain = objPosition.z < SPAWN_FIELD_DEPTH &&
                                                     objPosition.z > -SPAWN_FIELD_DEPTH &&
                                                     objPosition.x > -BORDER_OFFSET &&
                                                     objPosition.x < planeLimit + BORDER_OFFSET &&
                                                     objPosition.y > -BORDER_OFFSET &&
                                                     objPosition.y < planeLimit + BORDER_OFFSET;
                                 if (!shouldRemain) {
                                   delete obj;
                                 }
                                 return !shouldRemain;
                               }),
                objects.end());

  // filter stars
  stars.erase(std::remove_if(stars.begin(), stars.end(),
                             [](const Star *star) {
                               glm::vec3 pos = star->getPosition();
                               bool shouldRemain = pos.z < SPAWN_FIELD_DEPTH;
                               if (!shouldRemain) {
                                 delete star;
                               }
                               return !shouldRemain;
                             }),
              stars.end());

  // clip player coordinates
  glm::vec3 playerPosition = player->getPosition();
  float planeLimit = TRACKS_PER_DIRECTION * TRACK_SIZE;
  playerPosition.x = std::max(0.f, std::min(planeLimit, playerPosition.x));
  playerPosition.y = std::max(0.f, std::min(planeLimit, playerPosition.y));
  player->setPosition(playerPosition);
}

void Scene::addNewObjects(float deltaTime) {
  sinceLastStar += deltaTime;
  sinceLastTrackObject += deltaTime;

  if (sinceLastTrackObject > OBJECT_CREATION_TIMEOUT) {
    this->addTrackObject();
    sinceLastTrackObject = 0;
  }

  if (sinceLastStar > STAR_CREATION_TIMEOUT) {
    this->addStarObject();
    sinceLastStar = 0;
  }
}

void Scene::addTrackObject() {
  unsigned numberOfTracks = TRACKS_PER_DIRECTION * TRACKS_PER_DIRECTION;
  std::uniform_int_distribution<unsigned> trackDistribution(0, numberOfTracks - 1);
  std::uniform_int_distribution<unsigned> typeDistribution(0, 4);

  unsigned trackNumber = trackDistribution(generator);
  unsigned type = typeDistribution(generator);
  SPDLOG_INFO("Generating track object type <{}>", type);

  switch (type) {
  case 0:
    this->addObjectOnTrack(new Spaceship{100}, trackNumber);
    break;
  case 1:
    this->addObjectOnTrack(new Spaceship{200}, trackNumber);
    break;
  case 2:
    this->addObjectOnTrack(new Spaceship{300}, trackNumber);
    break;
  case 3:
    this->addObjectOnTrack(new Asteroid{100}, trackNumber);
    break;
  default:
    this->addObjectOnTrack(new Asteroid{200}, trackNumber);
    break;
  }
  SPDLOG_INFO("Created track object at {}", trackNumber);
}

void Scene::addStarObject() {
  float planeLimit = TRACKS_PER_DIRECTION * TRACK_SIZE;
  int spread = 2;
  int offset = 3;

  std::uniform_real_distribution<float> distribution(-spread * planeLimit, spread * planeLimit);

  float xRand = distribution(generator);
  float yRand = distribution(generator);
  float xPos = xRand > 0 ? xRand + (1 + offset) * planeLimit : xRand - offset * planeLimit;
  float yPos = yRand > 0 ? yRand + (1 + offset) * planeLimit : yRand - offset * planeLimit;

  stars.push_back(new Star{1000000, glm::vec3(xPos, yPos, -SPAWN_FIELD_DEPTH * 2)});
  SPDLOG_INFO("Created star at ({}, {})", xPos, yPos);
}

unsigned Scene::processCollisions() {
  // get a vector of all track object
  std::vector<Object *> trackObjects;
  std::for_each(tracks.begin(), tracks.end(), [&trackObjects](Track &track) {
    auto objs = track.getObjects();
    trackObjects.insert(trackObjects.end(), objs.begin(), objs.end());
  });

  // resolve track objects collisions
  for (Object *trackObj : trackObjects) {
    // with other track objects
    for (Object *otherTrackObj : trackObjects) {
      if (trackObj == otherTrackObj) {
        continue;
      }
      resolveCollision(trackObj, otherTrackObj);
    }

#ifndef GOD_MODE
    // with player
    resolveCollision(trackObj, player);
#endif
  }

  // resolve auxiliary object collisions
  for (Object *obj : objects) {
    // with other auxiliary objects
    for (Object *other : objects) {
      if (obj == other) {
        continue;
      }
      resolveCollision(obj, other);
    }

    // with track objects
    for (Object *other : trackObjects) {
      resolveCollision(obj, other);
    }

#ifndef GOD_MODE
    // with player
    resolveCollision(obj, player);
#endif
  }

  // remove all destroyed auxiliary objects
  unsigned objectsDestroyed = 0;
  this->removeDestroyed();
  std::for_each(tracks.begin(), tracks.end(),
                [&objectsDestroyed](Track &track) { objectsDestroyed += track.removeDestroyed(); });
  return objectsDestroyed;
}

void Scene::removeDestroyed() {
  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [](const Object *obj) {
                                 if (!obj->isAlive()) {
                                   delete obj;
                                   return true;
                                 }
                                 return false;
                               }),
                objects.end());
}

void Scene::createTorpedo(glm::vec3 initPos, glm::quat direction) {
  this->addObject(
      new Torpedo(TORPEDO_DAMAGE, initPos + 5.5f * QuatHelpers::getForward(direction), direction));
}