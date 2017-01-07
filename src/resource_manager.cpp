#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>

#include <SOIL.h>

// Instantiate static variables
std::map<std::string, Shader>       ResourceManager::Shaders;
std::vector<std::string>            ResourceManager::faces;
GLuint                              ResourceManager::skyboxTexture;


Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
     Shader shader(vShaderFile, fShaderFile);
    Shaders[name] = shader;
    return Shaders[name];
}

GLuint ResourceManager::LoadSkybox()
{
  faces.push_back("skybox/right.png");
  faces.push_back("skybox/left.png");
  faces.push_back("skybox/top.png");
  faces.push_back("skybox/bottom.png");
  faces.push_back("skybox/back.png");
  faces.push_back("skybox/front.png");
  skyboxTexture = loadCubemap(faces);
  return skyboxTexture;
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

GLuint ResourceManager::GetSkyboxTex()
{
    return skyboxTexture;
}

GLuint ResourceManager::loadCubemap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width, height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
