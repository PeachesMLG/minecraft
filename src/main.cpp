#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/Shader.h"
#include "Renderer/VAO.h"
#include "Renderer/EBO.h"
#include "Renderer/Vertex.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Vertex verticies[] = {
            -0.5f, -0.5f, 0.0f, 0.97f, 0.97f, 0.43f, // Lower left corner
            0.5f, -0.5f, 0.0f, 0.57f, 0.86f, 0.49, // Lower right corner
            -0.5f, 0.5f, 0.0f, 0.57f, 0.86f, 0.49f, // Upper left corner
            0.5f, 0.5f, 0.0f, 0.22f, 0.7f, 0.55f, // Upper right corner
    };

    GLuint indicies[] = {
            0, 1, 2,
            1, 2, 3
    };

    GLFWwindow *window = glfwCreateWindow(1200, 1200, "Minecraft", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

    VAO vao;
    vao.Bind();

    VBO vbo(verticies, sizeof(verticies));
    EBO ebo(indicies, sizeof(indicies));
    vao.LinkVBO(vbo);
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        double startTime = glfwGetTime();
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        ebo.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        double time = (glfwGetTime() - startTime) * 1000;
        std::string title = "Minecraft - FPS: " + std::to_string((int) (1000 / time));
        glfwSetWindowTitle(window, title.c_str());
    }

    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shaderProgram.Delete();

    glfwTerminate();
    return 0;
}
