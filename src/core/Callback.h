#ifndef SPACESHOOTER_CALLBACKS_H
#define SPACESHOOTER_CALLBACKS_H

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Callback {
public:
  // Error callback
  static void error(int code, const char *description);
};

#endif // SPACESHOOTER_CALLBACKS_H
