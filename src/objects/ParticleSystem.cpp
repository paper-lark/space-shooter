#include "ParticleSystem.h"
#include <spdlog/spdlog.h>

glm::vec3 ParticleSystem::getParticlePosition(const Particle &particle) const {
  return particle.offset + position;
}

void ParticleSystem::updateParticles(float deltaTime) {
  for (Particle &particle : particles) {
    particle.offset += velocity * deltaTime;
    particle.timeToLive -= deltaTime;
  }
}

void ParticleSystem::removeDead() {
  particles.erase(std::remove_if(particles.begin(), particles.end(),
                                 [](const Particle &p) { return p.timeToLive < 0; }),
                  particles.end());
}

void ParticleSystem::addParticle() {
  // FIXME: use a separate generator for angle and radius
  glm::vec3 offset = {
      distribution(generator),
      distribution(generator),
      0.f
  };
  particles.emplace_back(PARTICLE_TIME_TO_LIVE, offset);
}

void ParticleSystem::update(float deltaTime) {
  // update particles
  SPDLOG_INFO("Updating particle system");
  this->updateParticles(deltaTime);

  // add new particles
  timeSinceLastParticle += deltaTime;
  while (timeSinceLastParticle > spawnTimeout) {
    SPDLOG_INFO("Creating new particle");
    this->addParticle();
    timeSinceLastParticle -= spawnTimeout;
  } // FIXME: rewrite all timers that way

  // remove dead particles
  this->removeDead();
}

glm::mat4 ParticleSystem::getObjectModelMatrix(const Particle &particle) const {
  glm::mat4 translate = glm::translate(glm::mat4(1.f), this->getParticlePosition(particle));
  translate = glm::scale(translate, glm::vec3(PARTICLE_SCALE));
  return translate;
}

void ParticleSystem::draw(Shader shader) {
  SPDLOG_INFO("Drawing particle system with {} particles", particles.size());
  shader.use();
  for (const Particle &particle : particles) {
    shader.setMatrix("model", this->getObjectModelMatrix(particle));
    model->draw(shader);
  }
}

void ParticleSystem::setPosition(glm::vec3 aPos) {
  position = aPos;
}
