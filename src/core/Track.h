#ifndef SPACESHOOTER_TRACK_H
#define SPACESHOOTER_TRACK_H

#include "../objects/Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Track {
  std::vector<Object *> objects;
  std::vector<Object *> destructedObjects;
  glm::vec3 trackStart;
  glm::vec3 trackEnd;

public:
  // Constructor that accepts track start and end coordinates
  // Track is expected to be parallel to the z-axis
  explicit Track(glm::vec3 start = glm::vec3(0.f, 0.f, 0.f), glm::vec3 end = glm::vec3(0.f, 0.f, 0.f))
      : trackStart(start), trackEnd(end) {}

  // Destructor
  ~Track();

  // Copy constructor
  Track(const Track &track) : trackStart(track.trackStart), trackEnd(track.trackEnd) {}

  // Copy assignment
  Track &operator=(const Track &track) {
    trackStart = track.trackStart;
    trackEnd = track.trackEnd;
    return *this;
  }

  // Add new object to the track
  // Its position and orientation will be reset
  void add(Object *obj);

  // Remove object by pointer and frees its memory
  void remove(Object *obj);

  // Remove objects that out of track borders
  void checkBorders();

  // Remove destroyed objects from the track
  // Returns the number of destroyed objects
  unsigned removeDestroyed();

  // Update all objects on the track
  void update(float deltaTime);

  // Get objects
  std::vector<Object *> getObjects();

  // Draw track objects
  void draw(Shader shader);
};

#endif
