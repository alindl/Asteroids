#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>
#include "shader.h"

class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::vector<std::string>         faces;
    static GLuint                           skyboxTexture;
    static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);

    static GLuint LoadSkybox();
    // Retrieves a stored shader
    static Shader   GetShader(std::string name);

    static GLuint GetSkyboxTex();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // Loads and generates a shader from file
    static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    static GLuint loadCubemap(std::vector<std::string> faces);
};

#endif
