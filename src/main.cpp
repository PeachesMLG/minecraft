#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>
#include <iostream>
#include "Renderer/Shader.h"
#include "Renderer/VAO.h"
#include "Renderer/EBO.h"
#include "Renderer/Vertex.h"
#include "Player.h"
#include "Camera.h"

int width = 1200;
int height = 1200;
bool paused = false;
bool firstInput = true;
bool hasTriggeredPause = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Player &player) {
    double centerX = (double) width / 2;
    double centerY = (double) height / 2;
    float cameraSpeed = 0.25f;
    float sensitivity = 0.1f;

    if (firstInput) {
        glfwSetCursorPos(window, centerX, centerY);
        firstInput = false;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !hasTriggeredPause) {
        hasTriggeredPause = true;
        paused = !paused;
        if (paused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        hasTriggeredPause = false;
    }

    if (paused)return;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraSpeed *=2;

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glfwSetCursorPos(window, centerX, centerY);

    float xOffset = mouse_x - centerX;
    float yOffset = mouse_y - centerY;

    player.yaw += xOffset * sensitivity;
    player.pitch = std::max(std::min(player.pitch - (yOffset * sensitivity), 89.9f), -89.9f);

    glm::vec3 front = Camera::getFront(player);
    front.y = 0;
    front = glm::normalize(front);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player.position += cameraSpeed * front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player.position -= glm::normalize(glm::cross(front, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player.position -= cameraSpeed * front;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player.position += glm::normalize(glm::cross(front, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player.position.y += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        player.position.y -= 0.1;
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    Player player;

    while (!glfwWindowShouldClose(window)) {
        double startTime = glfwGetTime();
        processInput(window, player);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::cout << glm::to_string(player.position) << "(" << player.yaw << "/" << player.pitch << ")" << std::endl;

        shaderProgram.Activate();

        Camera::applyView(player, shaderProgram.ID, (float) (width / height));

        for (int x = 0; x < 16; ++x) {
            for (int z = 0; z < 16; ++z) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, 0.0f, z));
                int modelLog = glGetUniformLocation(shaderProgram.ID, "model");
                glUniformMatrix4fv(modelLog, 1, GL_FALSE, glm::value_ptr(model));

                ebo.Bind();
                vao.Bind();
                glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);
            }
        }

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
