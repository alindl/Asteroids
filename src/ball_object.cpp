#include "ball_object.h"

BallObject::BallObject()
    : GameObject(), Radius(2.0f), Stuck(true), model("lazer/lazer.obj")  { }

BallObject::BallObject(glm::vec3 pos, GLfloat radius)
    :  GameObject(pos, glm::vec3(radius * 2, radius * 2, radius * 2)), Radius(radius), Stuck(true), model("lazer/lazer.obj") { }

glm::vec3 BallObject::Move(GLfloat dt, Camera camera)
{
    // If not stuck to player
    if (!this->Stuck)
    {
        // Move the ball
        this->Position += this->Velocity * dt * 200.0f;
    }
    else
    {
      this->Position = camera.Position;
    }
    return this->Position;
}

void BallObject::Shoot(GLfloat dt, Camera camera)
{
  if (this->Stuck)
  {
    this->Stuck = false;
    glm::vec3 oldPos = this->Position;
    this->Position += camera.Front;
    this->Velocity = glm::vec3(this->Position - oldPos) * 5.0f;
  }
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(Camera camera)
{
    this->Position = camera.Position;
    this->Velocity = glm::vec3(0.0f);
    this->Stuck = true;
}

void BallObject::Draw(SpriteRenderer &renderer, Camera camera)
{
    renderer.DrawLazer(this->model, camera,this->Velocity ,this->Position);
}
