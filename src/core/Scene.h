#ifndef SPACESHOOTER_SCENE_H
#define SPACESHOOTER_SCENE_H

#include "../objects/Object.h"
#include "../objects/ParticleSystem.h"
#include "../objects/Player.h"
#include "../objects/Star.h"
#include "../representation/Skybox.h"
#include "Track.h"
#include <chrono>
#include <random>

#define TRACK_SIZE 10.f
#define TRACKS_PER_DIRECTION 5
#define SPAWN_FIELD_DEPTH 250.f
#define WARP_EFFECT_RADIUS 3.f
#define WARP_EFFECT_OFFSET glm::vec3(0.f, 0.f, -10.f)
#define WARP_EFFECT_VELOCITY glm::vec3(0.f, 0.f, 20.f)
#define WARP_EFFECT_SPAWN_TIMEOUT 0.05f

class Scene {
  Player *player;
  std::array<Track, TRACKS_PER_DIRECTION * TRACKS_PER_DIRECTION> tracks;
  std::vector<Object *> objects;
  std::vector<Star *> stars;
  Skybox *skybox;
  ParticleSystem particles = {WARP_EFFECT_OFFSET, WARP_EFFECT_VELOCITY, WARP_EFFECT_RADIUS, WARP_EFFECT_SPAWN_TIMEOUT};

  Shader objShader = Shader("object/vertex.glsl", "object/geometry.glsl", "object/fragment.glsl");
  Shader simpleShader = Shader("simple/vertex.glsl", "simple/fragment.glsl");

  // object generation properties
  // TODO: refactor into a separate class
  std::default_random_engine generator{
      static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())};
  float sinceLastTrackObject = 0.f;
  float sinceLastStar = 0.f;

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

  // Add track object
  void addTrackObject();

  // Add star object
  void addStarObject();

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
