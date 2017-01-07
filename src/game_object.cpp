#include "game_object.h"


GameObject::GameObject() 
    : Position(0, 0, 0), Size(1, 1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), IsSolid(GL_FALSE), Destroyed(GL_FALSE) { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size)
    : Position(pos), Size(size), Velocity(0.0f), Color(1.0f), Rotation(0.0f), IsSolid(GL_FALSE), Destroyed(GL_FALSE) { }

void GameObject::Draw(SpriteRenderer &renderer)
{ }
