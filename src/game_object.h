#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "sprite_renderer.h"

class GameObject
{
public:
    // Object state
    glm::vec3   Position, Size, Velocity;
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;
    // Constructor(s)
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 size);
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif
