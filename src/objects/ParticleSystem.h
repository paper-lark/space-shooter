#ifndef SPACESHOOTER_PARTICLESYSTEM_H
#define SPACESHOOTER_PARTICLESYSTEM_H

#include "../representation/Model.h"
#include "../representation/Shader.h"
#include <chrono>
#include <glm/vec3.hpp>
#include <random>
#include <vector>

#define PARTICLE_TIME_TO_LIVE 15.f
#define PARTICLE_SCALE .05f

struct Particle {
  float timeToLive;
  glm::vec3 offset;

  Particle(float timeToLive, glm::vec3 offset) : timeToLive(timeToLive), offset(offset) {}
};

class ParticleSystem {
  glm::vec3 position;
  glm::vec3 velocity;
  std::vector<Particle> particles;
  float timeSinceLastParticle = 0.f;
  std::default_random_engine generator{
      static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())};
  std::normal_distribution<float> distribution;
  Model *model = nullptr;
  const float spawnTimeout;

  // Calculate particle position
  glm::vec3 getParticlePosition(const Particle &particle) const;

  // Update particles' time to live
  void updateParticles(float deltaTime);

  // Remove dead particles
  void removeDead();

  // Add particle
  void addParticle();

  // Create model matrix for particle
  glm::mat4 getObjectModelMatrix(const Particle &particle) const;

public:
  // Constructor
  ParticleSystem(glm::vec3 position, glm::vec3 velocity, float radius, float spawnTimeout)
      : position(position), velocity(velocity), distribution(0, radius),
        model(new Model("assets/Star [1]/Mercury 1K.obj")), spawnTimeout(spawnTimeout) {}

  // Set particle system position
  void setPosition(glm::vec3 aPos);

  // Update particle system
  void update(float deltaTime);

  // Draw particle system
  void draw(Shader shader);
};

#endif
