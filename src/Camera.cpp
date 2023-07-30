#include "Camera.h"

void Camera::applyView(Player player, GLuint shaderId, float aspect) {
    glm::mat4 view = getView(player);
    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    int viewLog = glGetUniformLocation(shaderId, "view");
    glUniformMatrix4fv(viewLog, 1, GL_FALSE, glm::value_ptr(view));
    int projLog = glGetUniformLocation(shaderId, "proj");
    glUniformMatrix4fv(projLog, 1, GL_FALSE, glm::value_ptr(proj));
}

glm::mat4 Camera::getView(Player player) {
    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = getFront(player);
    glm::vec3 Right = glm::normalize(glm::cross(front, WorldUp));
    glm::vec3 Up = glm::normalize(glm::cross(Right, front));

    return glm::lookAt(player.position, player.position + front, Up);
}

glm::vec3 Camera::getFront(Player player) {
    glm::vec3 front;
    front.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
    front.y = sin(glm::radians(player.pitch));
    front.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
    return glm::normalize(front);
}