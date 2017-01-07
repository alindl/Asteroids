#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "game_object.h"
#include "camera.h"
#include "model.h"

class BallObject : public GameObject
{
public:
    // Ball state
    GLfloat   Radius;
    GLboolean Stuck;
    Model     model;
    // Constructor(s)
    BallObject();
    BallObject(glm::vec3 pos, GLfloat radius);
    glm::vec3 Move(GLfloat dt, Camera camera);
    void      Shoot(GLfloat dt, Camera camera);
    // Resets the ball to original state with given position and velocity
    void      Reset(Camera camera);
    void      Draw(SpriteRenderer &renderer, Camera camera);
};

#endif
