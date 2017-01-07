#include "game_level.h"

#include <fstream>
#include <sstream>
#include <algorithm>

void GameLevel::Load(GLuint amount, GLfloat radius, GLfloat offset)
{
    // Clear old data
    this->Asteroids.clear();
    GameLevel level;
    this->modelMatrices = new glm::mat4[amount];
    if (amount > 0)
      this->init(amount, radius, offset);
}

void GameLevel::Draw(SpriteRenderer &renderer, Camera camera)
{
    for(int i = 0; i < this->Asteroids.size(); i++)
    {
      AsterObject &asteroid = this->Asteroids[i];
      if (!asteroid.Destroyed)
          asteroid.Draw(renderer, camera, i);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, 0);
    }
}

GLboolean GameLevel::IsCompleted()
{
    for (AsterObject &asteroid : this->Asteroids)
        if (!asteroid.Destroyed)
            return GL_FALSE;
    return GL_TRUE;
}

void GameLevel::init(GLuint amount, GLfloat radius, GLfloat offset)
{
  srand(glfwGetTime());
  Model rock("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/asteroid/asteroid.obj");
  for(GLuint i = 0; i < amount; i++)
      {
        glm::mat4 model;
        // 1. Translation: Randomly displace along circle with radius 'radius' in range [-offset, offset]
        GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
        GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat x = sin(angle) * radius + displacement;
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat y = -2.5f + displacement * 0.4f; // Keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. Scale: Scale between 0.05 and 0.25f
        GLfloat scale = (rand() % 20) / 10000.0f + 5.00; // Die Größe der Asteroiden
        model = glm::scale(model, glm::vec3(scale));

        // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
        GLfloat rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. Now add to list of matrices
        this->modelMatrices[i] = model;
      }

      // forward declare the buffer
      GLuint buffer;
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &this->modelMatrices[0], GL_STATIC_DRAW);

      for(GLuint i = 0; i < rock.meshes.size(); i++)
      {
        GLuint VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // Set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
      }

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      for(GLuint i = 0; i < amount; i++){
        AsterObject obj(glm::vec3(0.0f), radius);
        glm::vec4 minBox = glm::vec4(-2.0f, -2.0f, -2.0f, 1.0f);
        glm::vec4 maxBox = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
        minBox =  modelMatrices[i] * minBox;
        maxBox =  modelMatrices[i] * maxBox;
        obj.minBox = minBox;
        obj.maxBox = maxBox;
        this->Asteroids.push_back(obj);
      }
}
