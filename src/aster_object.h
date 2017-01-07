#ifndef ASTEROBJECT_H
#define ASTEROBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "game_object.h"
#include "camera.h"
#include "model.h"


class AsterObject : public GameObject
{
public:
    // Ball state
    GLfloat   Radius;
    Model     model;
    glm::vec4 maxBox;
    glm::vec4 minBox;
    // Constructor(s)
    AsterObject();
    AsterObject(glm::vec3 pos, GLfloat radius);
    void Draw(SpriteRenderer &renderer, Camera camera, GLuint index);
    glm::vec3 Move(GLfloat dt);
};

#endif
