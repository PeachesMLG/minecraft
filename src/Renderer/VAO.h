#ifndef MINECRAFT_VAO_H
#define MINECRAFT_VAO_H

#include<glad/glad.h>
#include"VBO.h"

class VAO {
public:
    GLuint ID;

    VAO();

    void LinkVBO(VBO &vbo);

    void Bind();

    void Unbind();

    void Delete();
};

#endif //MINECRAFT_VAO_H
