#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include "Player.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    void applyView(Player player, GLuint shaderId, float aspect);

private:
    glm::mat4 getView(Player player);
};


#endif //MINECRAFT_CAMERA_H
