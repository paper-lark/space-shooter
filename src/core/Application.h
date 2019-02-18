#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Application {
private:
    GLFWwindow *window = nullptr;

public:
    // Constructor
    Application(GLFWwindow *window): window(window) {}

    // Process input
    void processInput() const;
};


#endif //MAIN_APPLICATION_H
