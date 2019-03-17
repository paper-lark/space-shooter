#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <iostream>
#include "utils/Texture.h"
#include "core/Application.h"
#include "core/Callback.h"
#include "utils/Shader.h"
#include "core/Camera.h"
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
}

// Initialize OpenGL
int initializeGL() {
    // load OpenGL with GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // output graphics adapter information
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);

    return 0;
}

// Create vertex buffer with a rectangle
GLuint createVertexBuffer() {
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject); // generate object
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); // bind buffer to a vertex buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // load vertex data into buffer

    return vertexBufferObject;
}

// Create element buffer object
// Using EBO allows indexed drawing to prevent storing redundant vertices when drawing complex shapes
GLuint createElementBufferObject() {
    unsigned int indices[] = {
            0, 1, 3,
            1, 3, 2,
            1, 2, 6,
            1, 5, 6,
            1, 5, 4,
            1, 0, 4,
            0, 3, 7,
            0, 4, 7,
            2, 6, 7,
            2, 3, 7,
            5, 4, 7,
            5, 6, 7
    };

    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return elementBufferObject;
}

// Create vertex array object
// Using VAO allows to configure vertex attribute pointers once and for all
GLuint createVertexArrayObject(GLuint vbo) {
    // create vertex array object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    createVertexBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // specify how to interpret vertex data from currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

// Create light source VAO
GLuint createLightArrayObject(GLuint vbo) {
    // create vertex array object
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // specify how to interpret vertex data from currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    return lightVAO;
}

// Get object model matrix (local -> world)
glm::mat4 getObjectModelMatrix() {
    glm::mat4 matrix = glm::mat4(1.f);
    matrix = glm::translate(matrix, glm::vec3(0.f, 0.f, 0.f));
    matrix = glm::rotate(matrix, glm::radians(30.f), glm::vec3(0.f, 1.f, 0.f));
    return matrix;
}

struct Material {
    glm::vec3 ambient; // TODO: remove as we use diffuse instead
    glm::vec3 diffuse; // TODO: remove as we use texture as diffuse map
    glm::vec3 specular; // TODO: remove as we use specular map
    float shininess;
};

Material getObjectMaterial() {
    // Read more: http://devernay.free.fr/cours/opengl/materials.html
    return Material{
        glm::vec3(0.135, 0.2225, 0.1575),
        glm::vec3(0.54, 0.89, 0.63),
        glm::vec3(0.316228, 0.316228, 0.316228),
        0.1
    };
};


struct Light {
    glm::vec4 vector;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

Light getLight() {
    return Light{
        glm::vec4(-1.f, 0.95f, 1.f, 1.f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        1.0, 0.09, 0.032
    };
}


// Get light source model matrix (local -> world)
glm::mat4 getLightModelMatrix(glm::vec3 lightPosition) {
    glm::mat4 matrix = glm::mat4(1.f);
    matrix = glm::translate(matrix, lightPosition);
    return glm::scale(matrix, glm::vec3(0.1f, 0.1f, 0.1f));
}

// Start game loop that ends when GLFW is signaled to close
void startGameLoop(GLFWwindow* window, Application &app) {
    // create prerequisites
    GLuint cube = createVertexBuffer();
    GLuint object = createVertexArrayObject(cube);
    GLuint light = createLightArrayObject(cube);
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f, 0.0f, 0.0f),
            glm::vec3( 2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f, 2.0f, -2.5f),
            glm::vec3( 1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // create textures and shaders
    Shader objShader = Shader("object/vertex.glsl", "object/fragment.glsl");
    Shader lightShader = Shader("light/vertex.glsl", "light/fragment.glsl");
    Texture objTexture = Texture("assets/container.jpg");
    Texture objSpecularMap = Texture("assets/container_specular.jpg");
    Texture objEmissionMap = Texture("assets/rune.jpg");
    Material objMaterial = getObjectMaterial();
    Light lightSpecs = getLight();

    while(!glfwWindowShouldClose(window)) {
        // update application
        app.update();

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw object
        objShader.use();
        objShader.setMatrix("view", app.camera.getViewMatrix());
        objShader.setMatrix("projection", app.camera.getProjectionMatrix());
        objShader.setVec3("viewPos", app.camera.getPos());
        objShader.setInt("material.diffuse", 0);
        objShader.setInt("material.specular", 1);
        objShader.setInt("material.emission", 2);
        objShader.setFloat("material.shininess", objMaterial.shininess);
        objShader.setVec4("light.vector", lightSpecs.vector);
        objShader.setVec3("light.diffuse", lightSpecs.diffuse);
        objShader.setVec3("light.ambient", lightSpecs.ambient);
        objShader.setVec3("light.specular", lightSpecs.specular);
        objShader.setFloat("light.constant", lightSpecs.constant);
        objShader.setFloat("light.linear", lightSpecs.linear);
        objShader.setFloat("light.quadratic", lightSpecs.quadratic);

        glBindVertexArray(object);
        objTexture.bind(GL_TEXTURE0);
        objSpecularMap.bind(GL_TEXTURE1);
        objEmissionMap.bind(GL_TEXTURE2);

        for (int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            objShader.setMatrix("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }


        // draw light source
        if (lightSpecs.vector.w == 1.f) {
            lightShader.use();
            lightShader.setMatrix("model", getLightModelMatrix(lightSpecs.vector));
            lightShader.setMatrix("view", app.camera.getViewMatrix());
            lightShader.setMatrix("projection", app.camera.getProjectionMatrix());
            lightShader.setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));
            glBindVertexArray(light);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // update color buffers
        glfwSwapBuffers(window); // swap front and back color buffers
        glfwPollEvents(); // process all events
    }

    // release resources
    glDeleteVertexArrays(1, &object);
    glDeleteVertexArrays(1, &light);
    glDeleteBuffers(1, &cube);
}

// TODO: read list
// https://learnopengl.com/Getting-started/Shaders
// https://learnopengl.com/In-Practice/Debugging
int main(int argc, char **argv) {
    // Create window
    initializeGLFW();
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create a window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // attach cursor to the window

    // Initialize application
    Application::initialize(window);

    // Register event callbacks
    glfwSetFramebufferSizeCallback(window, Callback::windowResize);
    glfwSetErrorCallback(Callback::error);
    glfwSetCursorPosCallback(
            window,
            [](GLFWwindow* window, double posX, double posY) {
                Application::getSingleton().processMouseInput(window, posX, posY);
            }
    );

    // Initialize OpenGL
    if (initializeGL() != 0) {
        return -1;
    }

    // Start game loop
    startGameLoop(window, Application::getSingleton());

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
