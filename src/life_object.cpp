#include "life_object.h"

LifeObject::LifeObject()
    : GameObject(), Radius(2.0f), model("life/life.obj")  { }

LifeObject::LifeObject(glm::vec3 pos, GLfloat radius)
    :  GameObject(pos, glm::vec3(radius * 2, radius * 2, radius * 2)), Radius(radius), model("life/life.obj") { }

glm::vec3 LifeObject::Move(GLfloat dt, Camera camera)
{
  this->Position = camera.Position + camera.Front * 5.0f;
      return this->Position;
}

void LifeObject::Draw(SpriteRenderer &renderer, Camera camera, GLfloat index)
{
    renderer.DrawLife(this->model, camera, this->Position + (camera.Front) + (camera.Up * 3.05f) + (camera.Right * 3.8f * index));
}
