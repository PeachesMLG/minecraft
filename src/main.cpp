#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
            -0.5f, -0.5f, -0.5f, 0.97f, 0.97f, 0.43f,     //        7--------6
            0.5f, -0.5f, -0.5f, 0.57f, 0.86f, 0.49,       //       /|       /|
            0.5f, -0.5f, 0.5f, 0.57f, 0.86f, 0.49f,     //      4--------5 |
            -0.5f, -0.5f, 0.5f, 0.22f, 0.7f, 0.55f,     //      | |      | |
            -0.5f, 0.5f, -0.5f, 0.57f, 0.86f, 0.49,       //      | 3------|-2
            0.5f, 0.5f, -0.5f, 0.57f, 0.86f, 0.49,        //      |/       |/
            0.5f, 0.5f, 0.5f, 0.57f, 0.86f, 0.49,       //      0--------1
            -0.5f, 0.5f, 0.5f, 0.57f, 0.86f, 0.49,
    };

    GLuint indicies[] = {
            // Right
            1, 2, 6,
            6, 5, 1,
            // Left
            0, 4, 7,
            7, 3, 0,
            // Top
            4, 5, 6,
            6, 7, 4,
            // Bottom
            0, 3, 2,
            2, 1, 0,
            // Front
            0, 1, 5,
            5, 4, 0,
            // Back
            3, 7, 6,
            6, 2, 3
    };

    int width = 1200;
    int height = 1200;

    GLFWwindow *window = glfwCreateWindow(width, height, "Minecraft", nullptr, nullptr);
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
    glEnable(GL_DEPTH_TEST);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double startTime = glfwGetTime();
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();

        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1.0f / 60.0f) {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float) (width / height), 0.1f, 100.0f);

        int modelLog = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLog, 1, GL_FALSE, glm::value_ptr(model));
        int viewLog = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLog, 1, GL_FALSE, glm::value_ptr(view));
        int projLog = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLog, 1, GL_FALSE, glm::value_ptr(proj));

        ebo.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);

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
