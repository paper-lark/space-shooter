#ifndef SPACESHOOTER_CALLBACKS_H
#define SPACESHOOTER_CALLBACKS_H

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Callback {

public:
  // Current window size
  static GLint windowSize[2]; // FIXME: remove

  // Window resize callback
  static void windowResize(GLFWwindow *, GLint width, GLint height);

  // Error callback
  static void error(int code, const char *description);
};

#endif // SPACESHOOTER_CALLBACKS_H
