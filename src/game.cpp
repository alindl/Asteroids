#include "game.h"

// Game-related State data
SpriteRenderer  *Renderer;
BallObject      *Lazer;
Camera          camera(glm::vec3(0.0f, 100.0f, 155.0f));

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ }

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    camera.ChangeSpeed(60.0f);
    liveCounter = 4;
    astCounter = 10;
    // Load shaders
    ResourceManager::LoadShader("asteroid/asteroid.vs", "asteroid/asteroid.frag", nullptr, "asteroid");
    ResourceManager::LoadShader("astCount/astCount.vs", "astCount/astCount.frag", nullptr, "count");
    ResourceManager::LoadShader("skybox/skybox.vs", "skybox/skybox.frag", nullptr, "skybox");
    ResourceManager::LoadShader("lazer/lazer.vs", "lazer/lazer.frag", nullptr, "lazer");
    ResourceManager::LoadShader("life/life.vs", "life/life.frag", nullptr, "life");

    glm::mat4 projection =  glm::perspective(45.0f, (GLfloat)Width/(GLfloat)Height, 1.0f, 10000.0f);

    ResourceManager::LoadSkybox();
    Shader skyboxShader, lazerShader, asteroidShader, lifeShader, countShader;
    skyboxShader = ResourceManager::GetShader("skybox");
    asteroidShader = ResourceManager::GetShader("asteroid");
    lazerShader = ResourceManager::GetShader("lazer");
    lifeShader = ResourceManager::GetShader("life");
    countShader = ResourceManager::GetShader("count");
    Renderer = new SpriteRenderer(skyboxShader, asteroidShader, lazerShader, lifeShader, countShader);
    Lazer = new BallObject(camera.Position, 5.0f);
    for (GLuint var = 1; var < liveCounter; ++var)
    {
      LifeObject *TempLife;
      TempLife = new LifeObject(camera.Position, 5.0f);
      Lives.push_back(TempLife);
    }
    for (GLuint var = 0; var < astCounter; ++var)
    {
      CountObject *TempCount;
      TempCount = new CountObject(camera.Position, 5.0f);
      Asts.push_back(TempCount);
    }

    asteroidShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    lazerShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(lazerShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    lifeShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(lifeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    countShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(countShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    GameLevel one; one.Load(astCounter, 150.0f, 250.0f);
    this->Levels.push_back(one);
    this->Level = 0;
    Renderer->modelMatrices = one.modelMatrices;

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Game::Update(GLfloat dt)
{
  // Update objects
  if(liveCounter > 0)
    for (LifeObject *temp : Lives)
    {
      temp->Move(dt, camera);
    }
  if(astCounter > 0)
    for (CountObject *temp : Asts)
    {
      temp->Move(dt, camera);
    }
  Lazer->Move(dt, camera);
  // Check for collisions
  this->DoCollisions();
  if (this->State == GAME_ACTIVE)
  {
    if(this->Levels[this->Level].IsCompleted() == GL_TRUE)
      this->State = GAME_WIN;
    if(liveCounter <= 0)
      this->State = GAME_LOSE;
  }
}

void Game::ProcessInput(GLfloat dt)
{
    // This is to ensure that it doesn't fire immediately again if an Asteroid is very close
    if(!this->Keys[GLFW_KEY_SPACE] && ShootBool)
    {
      ShootBool = false;
      Lazer->Shoot(dt,camera);
    }

    if(!this->Keys[GLFW_KEY_H] && CheatBool)
    {
      CheatBool = false;
      for (AsterObject &asteroid : this->Levels[this->Level].Asteroids)
      {
          if (!asteroid.Destroyed)
          {
            asteroid.Destroyed = GL_TRUE;
            astCounter--;
            Asts.pop_back();
            return;
          }
      }
    }
    if(this->Keys[GLFW_KEY_SPACE])
      ShootBool = true;
    if(this->Keys[GLFW_KEY_H])
      CheatBool = true;

    if(this->Keys[GLFW_KEY_W])
      camera.ProcessKeyboard(FORWARD, dt);
    if(this->Keys[GLFW_KEY_S])
      camera.ProcessKeyboard(BACKWARD, dt);
    if(this->Keys[GLFW_KEY_A])
      camera.ProcessKeyboard(LEFT, dt);
    if(this->Keys[GLFW_KEY_D])
      camera.ProcessKeyboard(RIGHT, dt);
    if(this->Keys[GLFW_KEY_UP])
      camera.ProcessKeyboard(PITCHUP, dt);
    if(this->Keys[GLFW_KEY_DOWN])
      camera.ProcessKeyboard(PITCHDOWN, dt);
    if(this->Keys[GLFW_KEY_LEFT])
      camera.ProcessKeyboard(YAWLEFT, dt);
    if(this->Keys[GLFW_KEY_RIGHT])
      camera.ProcessKeyboard(YAWRIGHT, dt);
    if(this->Keys[GLFW_KEY_LEFT_SHIFT])
      camera.ProcessKeyboard(BOOST, dt);
    if(!this->Keys[GLFW_KEY_LEFT_SHIFT])
      camera.ChangeSpeed(60.0f);
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
      GLfloat lifeInd = 1.1f;
      this->Levels[this->Level].Draw(*Renderer, camera);
      Lazer->Draw(*Renderer, camera);
      if(liveCounter > 0)
        for (LifeObject *temp : Lives)
        {
          temp->Draw(*Renderer, camera, lifeInd);
          lifeInd -= 0.04;
        }
      GLfloat astInd = 3.7f;
      if(astCounter > 0)
        for (CountObject *temp : Asts)
        {
          temp->Draw(*Renderer, camera, astInd);
          astInd -= 0.3;
        }
      GLuint mySkyTex;
      mySkyTex = ResourceManager::GetSkyboxTex();
      Renderer->DrawSkybox(mySkyTex ,camera);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    camera.Position = glm::vec3(0.0f, 100.0f, 155.0f);
    Lazer->Reset(camera);
}

void Game::DoCollisions()
{
    for (AsterObject &asteroid : this->Levels[this->Level].Asteroids)
    {
        if (!asteroid.Destroyed)
        {
            if (this->CheckCollisionAtoS(asteroid))
            {
              asteroid.Destroyed = GL_TRUE;
              this->ResetPlayer();
              Lazer->Reset(camera);
              liveCounter--;
              Lives.pop_back();
              astCounter--;
              Asts.pop_back();
            }
            if(!Lazer->Stuck)
            {
              if (this->CheckCollisionAtoL(asteroid, *Lazer))
              {
                asteroid.Destroyed = GL_TRUE;
                Lazer->Reset(camera);
                astCounter--;
                Asts.pop_back();
              }
            }
        }    
    }
    if (!Lazer->Stuck && CheckOOR(*Lazer))
    {
      Lazer->Reset(camera);
    }
}

GLboolean Game::CheckCollisionAtoS(AsterObject &one) // Box - Sphere collision
{
  GLfloat x = std::max(one.minBox.x, std::min(camera.Position.x,  one.maxBox.x));
  GLfloat y = std::max(one.minBox.y, std::min(camera.Position.y,  one.maxBox.y));
  GLfloat z = std::max(one.minBox.z, std::min(camera.Position.z,  one.maxBox.z));
  GLfloat distance = ((x - camera.Position.x) * (x - camera.Position.x)) +
                     ((y - camera.Position.y) * (y - camera.Position.y)) +
                     ((z - camera.Position.z) * (z - camera.Position.z));
  GLboolean boom = distance < camera.Radius;
    return boom;
}

GLboolean Game::CheckCollisionAtoL(AsterObject &one, BallObject &two) // Box - Sphere collision
{
  GLfloat x = std::max(one.minBox.x, std::min(two.Position.x,  one.maxBox.x));
  GLfloat y = std::max(one.minBox.y, std::min(two.Position.y,  one.maxBox.y));
  GLfloat z = std::max(one.minBox.z, std::min(two.Position.z,  one.maxBox.z));
  GLfloat distance = ((x - two.Position.x) * (x - two.Position.x)) +
                     ((y - two.Position.y) * (y - two.Position.y)) +
                     ((z - two.Position.z) * (z - two.Position.z));
  GLboolean boom = distance < two.Radius;
  return boom;
}

GLboolean Game::CheckOOR(BallObject &one) // Ball too far away
{
  GLfloat distance =  ((camera.Position.x - one.Position.x) * (camera.Position.x - one.Position.x) +
                       (camera.Position.y - one.Position.y) * (camera.Position.y - one.Position.y) +
                       (camera.Position.z - one.Position.z) * (camera.Position.z - one.Position.z));
  GLboolean boom = distance > 10000000; // Some value for a far enough distance until reset
  return boom;
}
