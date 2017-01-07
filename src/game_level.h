#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "game_object.h"
#include "aster_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


class GameLevel
{
public:
    // Level state
    std::vector<AsterObject> Asteroids;
    glm::mat4* modelMatrices;
    // Constructor
    GameLevel() { }
    void      Load(GLuint amount, GLfloat radius, GLfloat offset);
    void      Draw(SpriteRenderer &renderer, Camera camera);
    GLboolean IsCompleted();
private:
    void      init(GLuint amount, GLfloat radius, GLfloat offset);

};

#endif
