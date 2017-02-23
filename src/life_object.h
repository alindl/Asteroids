#ifndef LIFEOBJECT_H
#define LIFEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "game_object.h"
#include "camera.h"
#include "model.h"

class LifeObject : public GameObject
{
public:
    // Ball state
    GLfloat   Radius;
    Model     model;
    // Constructor(s)
    LifeObject();
    LifeObject(glm::vec3 pos, GLfloat radius);
    void      Draw(SpriteRenderer &renderer, Camera camera, GLfloat index);
    glm::vec3 Move(GLfloat dt, Camera camera);
};

#endif
