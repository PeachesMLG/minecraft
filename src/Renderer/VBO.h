#ifndef MINECRAFT_VBO_H
#define MINECRAFT_VBO_H

#include<glad/glad.h>
#include "Vertex.h"

class VBO {
public:
    GLuint ID;

    VBO(Vertex *vertices, GLsizeiptr size);

    void Bind();

    void Unbind();

    void Delete();
};


#endif //MINECRAFT_VBO_H
