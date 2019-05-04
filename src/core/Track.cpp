#include "Track.h"
#include "../objects/Spaceship.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#define DELETE_DELAY 3.f

Track::~Track() {
  SPDLOG_INFO("Destructor called");
  std::for_each(objects.begin(), objects.end(), [](Object *obj) { delete obj; });
  objects.clear();
}

void Track::add(Object *obj) {
  SPDLOG_INFO("Adding object...");
  obj->setOrientation(glm::angleAxis(0.f, glm::vec3(1, 0, 0)));
  obj->setPosition(trackStart);
  objects.push_back(obj);
}

void Track::remove(Object *obj) {
  auto it = std::find(objects.begin(), objects.end(), obj);
  if (it != objects.end()) {
    objects.erase(it);
    delete *it;
    SPDLOG_INFO("Object removed");
  }
}

void Track::checkBorders() {
  std::vector<Object *> filtered;
  double limit = trackEnd.z;
  std::copy_if(objects.begin(), objects.end(), std::back_inserter(filtered),
               [limit](const Object *obj) {
                 bool result = obj->getPosition().z < limit;
                 if (!result) {
                   delete obj;
                 }
                 return result;
               });
  objects = filtered;
}

std::vector<Object *> Track::getObjects() {
  return objects;
}

unsigned Track::removeDestroyed() {
  float currentTime = glfwGetTime();
  // find destroyed track objects
  unsigned destroyed = 0;
  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [&destroyed, currentTime, this](Object *obj) {
                                 if (!obj->isAlive()) {
                                   destroyed++;
                                   destructedObjects.emplace_back(obj, currentTime);
                                   return true;
                                 }
                                 return false;
                               }),
                objects.end());

  // remove outdated destroyed objects
  destructedObjects.erase(std::remove_if(destructedObjects.begin(), destructedObjects.end(),
                                         [currentTime](std::pair<Object *, float> p) {
                                           bool shouldDelete =
                                               currentTime > DELETE_DELAY + std::get<1>(p);
                                           if (shouldDelete) {
                                             delete std::get<0>(p);
                                           }
                                           return shouldDelete;
                                         }),
                          destructedObjects.end());
  return destroyed;
}

void Track::update(float deltaTime) {
  // update track objects
  std::for_each(objects.begin(), objects.end(), [deltaTime, this](Object *obj) {
    if (obj == *objects.begin()) {
      auto ship = dynamic_cast<Spaceship *>(obj);
      if (ship != nullptr) {
        // first ship on the track should shoot
        ship->update(deltaTime, true);
      }
    }
    obj->update(deltaTime);
  });

  // update destroyed objects
  std::for_each(destructedObjects.begin(), destructedObjects.end(),
                [deltaTime](std::pair<Object *, float> p) { std::get<0>(p)->update(deltaTime); });
}

void Track::draw(Shader shader) {
  std::for_each(objects.cbegin(), objects.cend(),
                [&shader](const Object *obj) { obj->draw(shader); });
  std::for_each(destructedObjects.cbegin(), destructedObjects.cend(),
                [&shader](const std::pair<Object *, float> p) { std::get<0>(p)->draw(shader); });
}