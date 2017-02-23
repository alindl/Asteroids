#include "aster_count.h"

CountObject::CountObject()
    : GameObject(), Radius(2.0f), model("astCount/astCount.obj")  { }

CountObject::CountObject(glm::vec3 pos, GLfloat radius)
    :  GameObject(pos, glm::vec3(radius * 2, radius * 2, radius * 2)), Radius(radius), model("astCount/astCount.obj") { }

glm::vec3 CountObject::Move(GLfloat dt, Camera camera)
{
  this->Position = camera.Position + camera.Front * 5.0f;
      return this->Position;
}

void CountObject::Draw(SpriteRenderer &renderer, Camera camera, GLfloat index)
{
    renderer.DrawCount(this->model, camera, this->Position + (camera.Front) + (camera.Up * 3.0f) + ((-camera.Right) * 1.1f * index));
}
