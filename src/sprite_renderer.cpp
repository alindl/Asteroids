#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &skyboxShader, Shader &asteroidShader, Shader &lazerShader)
{
  this->skyboxShader = skyboxShader;
  this->initSkyboxRenderData();

  this->lazerShader = lazerShader;

  this->asteroidShader = asteroidShader;
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->asteroidVAO);
    glDeleteVertexArrays(1, &this->skyboxVAO);
}

void SpriteRenderer::DrawLazer(Model lazer, Camera camera, glm::vec3 velocity, glm::vec3 pos)
{
    // Prepare transformations
    this->lazerShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(lazerShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

    glm::mat4 shootModel;
    shootModel = glm::translate(shootModel, velocity + pos);
    glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(shootModel));

    glUniform3f(glGetUniformLocation(lazerShader.Program, "spriteColor"), 1.0f, 0.0f, 0.0f);
    lazer.Draw(lazerShader);
}

void SpriteRenderer::DrawAsteroid(Model rock, Camera camera, GLuint index)
{
    // Prepare transformations
    this->asteroidShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

    glm::mat4 model = this->modelMatrices[index];
    glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    rock.Draw(asteroidShader);
}

void SpriteRenderer::DrawSkybox(GLuint texture, Camera camera)
{
  // Prepare transformations
  this->skyboxShader.Use();

  glm::mat4 projection, view;
  view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
  projection = glm::perspective(45.0f, (GLfloat)800/(GLfloat)600, 1.0f, 10000.0f);

  glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

  glBindVertexArray(this->skyboxVAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}

void SpriteRenderer::initSkyboxRenderData()
{
    // Configure VAO/VBO
    GLuint skyboxVBO;
    GLfloat skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f,    1.0f,  1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,   -1.0f,  1.0f,  1.0f,   -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,    1.0f, -1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,   -1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f, -1.0f,  1.0f,   -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,    1.0f,  1.0f, -1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,   -1.0f,  1.0f,  1.0f,   -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f,  1.0f,    1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f,   -1.0f, -1.0f,  1.0f,    1.0f, -1.0f,  1.0f};

    glGenVertexArrays(1, &this->skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(this->skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
