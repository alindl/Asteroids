#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "model.h"
#include "shader.h"

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
  SpriteRenderer(Shader &skyboxShader, Shader &asteroidShader, Shader &lazerShader);
    // Destructor
    ~SpriteRenderer();
    void DrawAsteroid(Model rock, Camera camera, GLuint index);
    void DrawLazer(Model lazer, Camera camera, glm::vec3 velocity, glm::vec3 pos);
    void DrawSkybox(GLuint texture, Camera camera);
    glm::mat4* modelMatrices;
private:
    // Render state
    Shader asteroidShader;
    GLuint asteroidVAO;

    Shader lazerShader;
    GLuint lazerVAO;

    Shader skyboxShader;
    GLuint skyboxVAO;

    void initSkyboxRenderData();
};

#endif
