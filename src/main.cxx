#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO

#include "core/Application.h"
#include "core/Callback.h"
#include "core/Camera.h"
#include "core/HUD.h"
#include "core/Scene.h"
#include "objects/Player.h"
#include "objects/Spaceship.h"
#include "objects/Star.h"
#include "representation/Model.h"
#include "representation/Shader.h"
#include "representation/Skybox.h"
#include "representation/Texture.h"
#include "utils/Box.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include <spdlog/spdlog.h>

#define length(x) (sizeof(x) / sizeof((x)[0]))
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define WINDOW_TITLE "OpenGL Introduction"

// Initialize GLFW
void initializeGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
}

// Initialize OpenGL
int initializeGL() {
  // load OpenGL with GLAD
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    return -1;
  }

  // output graphics adapter information
  SPDLOG_DEBUG("Device info: {}", glGetString(GL_RENDERER));
  SPDLOG_DEBUG("OpenGL Version: {}, {}", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

  // enable depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // enable face culling
  glEnable(GL_CULL_FACE);

  // enable multisampling
  glEnable(GL_MULTISAMPLE);

  return 0;
}

// Get light source model matrix (local -> world)
glm::mat4 getLightModelMatrix(const glm::vec3 position) {
  glm::mat4 matrix = glm::mat4(1.f);
  matrix = glm::translate(matrix, position);
  matrix = glm::scale(matrix, glm::vec3(30, 30, 30));
  return matrix;
}

// Start game loop that ends when GLFW is signaled to close
void startGameLoop(GLFWwindow *window, Application &app) {
  // create prerequisites
  HUD hud("assets/Fonts/ShareTechMono.ttf");
  Shader hudShader = Shader("hud/vertex.glsl", "hud/fragment.glsl");
  Scene scene{};

  while (!glfwWindowShouldClose(window)) {
    // update application
    app.update();

    // clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(unsigned(GL_COLOR_BUFFER_BIT) | unsigned(GL_DEPTH_BUFFER_BIT));

    // draw scene and HUD
    scene.draw(app.camera);
    hud.Draw(hudShader, app.getWindowSize(), scene.getPlayer()->getHealth(), app.getScore());

    // update scene
    scene.update(app.getDeltaTime());
    // TODO: check player health

    // update color buffers
    glfwSwapBuffers(window); // swap front and back color buffers
    glfwPollEvents();        // process all events
  }

  // unbind player
  app.bindPlayer(nullptr);
}

// Entry point
int main(int, char **) {
  // Initialize logger
  spdlog::set_pattern("%T – %l [%s] %v");
  SPDLOG_INFO("Initializing application...");
  SPDLOG_DEBUG("Some debug message");

  // Create window
  initializeGLFW();
  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
  if (window == nullptr) {
    SPDLOG_ERROR("Failed to create a window, terminating...");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize application
  Application::initialize(window);

  // Register event callbacks
  glfwSetErrorCallback(Callback::error);
  glfwSetCursorPosCallback(window, [](GLFWwindow *window, double posX, double posY) {
    Application::getSingleton().processMouseInput(window, posX, posY);
  });

  // Initialize OpenGL
  if (initializeGL() != 0) {
    SPDLOG_ERROR("Failed to initialize OpenGL");
    return -1;
  }

  // Load all models
  SPDLOG_INFO("Loading models...");
  Spaceship::init();
  Player::init();
  Star::init();

  // Start game loop
  SPDLOG_INFO("Starting game loop...");
  startGameLoop(window, Application::getSingleton());

  // Terminate GLFW
  SPDLOG_INFO("Terminating...");
  Spaceship::release();
  Player::release();
  Star::release();
  glfwTerminate();
  return 0;
}
