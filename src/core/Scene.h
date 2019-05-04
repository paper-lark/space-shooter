#ifndef SPACESHOOTER_SCENE_H
#define SPACESHOOTER_SCENE_H

#include "../objects/Object.h"
#include "../objects/Player.h"
#include "../objects/Star.h"
#include "../representation/Skybox.h"
#include "Track.h"

#define TRACK_SIZE 10.f
#define TRACKS_PER_DIRECTION 5
#define SPAWN_FIELD_DEPTH 250.f

class Scene {
  Player *player;
  std::array<Track, TRACKS_PER_DIRECTION * TRACKS_PER_DIRECTION> tracks;
  std::vector<Object *> objects;
  std::vector<Star *> stars;
  Skybox *skybox;

  Shader objShader = Shader("object/vertex.glsl", "object/fragment.glsl");
  Shader lightShader = Shader("light/vertex.glsl", "light/fragment.glsl");

  // Remove auxiliary objects and stars out of borders and clip player's coordinates
  void checkBorders();

  // Add new objects, including stars and track objects
  void addNewObjects(float deltaTime);

  // Add object on track
  void addObjectOnTrack(Object *obj, unsigned trackNumber);

  // Process all object collisions
  // Returns number of destroyed track objects
  unsigned processCollisions();

  // Remove all destructed auxiliary objects
  void removeDestroyed();

public:
  // Constructor
  Scene();

  // Destructor
  ~Scene();

  // Add an auxiliary object to the list
  void addObject(Object *obj);

  // Draw scene objects
  void draw(Camera &camera);

  // Update scene
  // Returns number of destroyed track objects
  unsigned update(float deltaTime);

  // Get player on the scene
  Player *getPlayer();

  // Create torpedo
  void createTorpedo(glm::vec3 initPos, glm::quat direction);
};



#endif //SPACESHOOTER_SCENE_H
