/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef LAZER_H
#define LAZER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

//#include "texture.h"
//#include "sprite_renderer.h"
#include "game_object.h"

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's ball object that
// were too specific for within GameObject alone.
class Lazer : public GameObject
{
public:
    // Ball state
    GLfloat   Radius;
    GLboolean Stuck;
    // Constructor(s)
    Lazer();

    Lazer(glm::vec3 pos, GLfloat radius, glm::vec3 velocity
//        , Texture2D sprite
        );
    // Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec3 Move(GLfloat dt);
    // Resets the ball to original state with given position and velocity
    void      Reset(glm::vec3 position, glm::vec3 velocity);
};

#endif
