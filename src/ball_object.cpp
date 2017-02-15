#include "ball_object.h"

BallObject::BallObject()
    : GameObject(), Radius(2.0f), Stuck(true), model("lazer/lazer.obj")  { }

BallObject::BallObject(glm::vec3 pos, GLfloat radius)
    :  GameObject(pos, glm::vec3(radius * 2, radius * 2, radius * 2)), Radius(radius), Stuck(true), model("lazer/lazer.obj") { }

glm::vec3 BallObject::Move(GLfloat dt, Camera camera)
{
    // If not stuck to player ship
    if (!this->Stuck)
    {
        // Move the Lazer
        this->Position += this->Velocity * dt * 150.0f;
    }
    else
    {// Lazer to player ship position
      this->Position = camera.Position;
    }
    return this->Position;
}

void BallObject::Shoot(GLfloat dt, Camera camera)
{
  if (this->Stuck)// If Lazer is at the player ship
  {
    this->Stuck = false;
    this->Position += camera.Front;
    this->Velocity = camera.Front * 5.0f;
  }
}

// Resets the ball to initial Stuck Position at the ship
void BallObject::Reset(Camera camera)
{
    this->Position = camera.Position;
    this->Velocity = glm::vec3(0.0f);
    this->Stuck = true;
}

void BallObject::Draw(SpriteRenderer &renderer, Camera camera)
{
    renderer.DrawLazer(this->model, camera, this->Velocity, this->Position);
}
