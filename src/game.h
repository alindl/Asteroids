#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "aster_object.h"
#include "ball_object.h"
#include "aster_count.h"
#include "life_object.h"
#include "camera.h"
#include "shader.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_WIN,
    GAME_LOSE
};

class Game
{
public:
    // Game state
    GameState                  State;
    GLboolean                  Keys[1024];
    GLuint                     Width, Height;
    std::vector<GameLevel>     Levels;
    std::vector<LifeObject*>   Lives;
    std::vector<CountObject*>  Asts;
    GLuint                     Level;
    GLboolean                  ShootBool = false;
    GLboolean                  CheatBool = false;
    GLuint                     liveCounter;
    GLuint                     astCounter;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    void DoCollisions();
    // Reset
    void ResetLevel();
    void ResetPlayer();

    GLboolean CheckCollisionAtoL(AsterObject &one, BallObject &two);
    GLboolean CheckCollisionAtoS(AsterObject &one);
    GLboolean CheckOOR(BallObject &one);
};

#endif
