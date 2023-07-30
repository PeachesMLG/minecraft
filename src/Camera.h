#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include "Player.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    static void applyView(Player player, GLuint shaderId, float aspect);
    static glm::vec3 getFront(Player player);

private:
    static glm::mat4 getView(Player player);
};


#endif //MINECRAFT_CAMERA_H
