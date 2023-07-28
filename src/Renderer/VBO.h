#ifndef MINECRAFT_VBO_H
#define MINECRAFT_VBO_H

#include<glad/glad.h>

class VBO {
public:
    GLuint ID;

    VBO(GLfloat *vertices, GLsizeiptr size);

    void Bind();

    void Unbind();

    void Delete();
};


#endif //MINECRAFT_VBO_H
