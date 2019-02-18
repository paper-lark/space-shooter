#ifndef MAIN_CALLBACKS_H
#define MAIN_CALLBACKS_H

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

class Callback {

public:
    // Window resize callback
    static void windowResize(GLFWwindow* _, GLint width, GLint height);

    // Error callback
    static void error(int code, const char* description);
};


#endif //MAIN_CALLBACKS_H
