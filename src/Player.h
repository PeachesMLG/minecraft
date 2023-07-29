#ifndef MINECRAFT_PLAYER_H
#define MINECRAFT_PLAYER_H


#include "glm/vec3.hpp"

class Player {
public:
    glm::vec3 position = {0.0f, 0.0f,-2.0f};
    float yaw = 0;
    float pitch = 0;
};


#endif //MINECRAFT_PLAYER_H
