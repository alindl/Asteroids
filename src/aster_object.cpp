#include "aster_object.h"

AsterObject::AsterObject()
    : GameObject(), Radius(2.0f), model("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/asteroid/asteroid.obj") { }

AsterObject::AsterObject(glm::vec3 pos, GLfloat radius)
    :  GameObject(pos, glm::vec3(radius * 2, radius * 2, radius * 2)), Radius(radius), model("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/asteroid/asteroid.obj")  { }

glm::vec3 AsterObject::Move(GLfloat dt)
{
    return this->Position;
}
void AsterObject::Draw(SpriteRenderer &renderer, Camera camera, GLuint index)
{
    renderer.DrawAsteroid(this->model, camera, index);
}
