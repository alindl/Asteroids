/* standard includes */
#include<stdio.h>
#include<stdlib.h>

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
#include <SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
#include<GLFW/glfw3.h>
/* GLNM is used for handling vector and matrix math */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL(src) "#version 330 core\n" #src
#define GLM_FORCE_RADIANS

/*                                                                           */
/* GLFW callback functions for event handling                                */
/*                                                                           */
static void errorCallback(int error, const char* description)
{
   fprintf(stderr, "GLFW error %d: %s\n", error, description);
}


static void keyCallback(GLFWwindow* myWindow, int key, int scanCode, 
                         int action, int mod)
{
   if (((key == GLFW_KEY_ESCAPE) || (key == GLFW_KEY_Q))  && 
       (action == GLFW_PRESS))
      /* close window upon hitting the escape key or Q/q */
      glfwSetWindowShouldClose(myWindow, GL_TRUE);
}


bool checkShaderCompileStatus(GLuint shaderID) 
{
   GLint status;
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
   if(status == GL_FALSE) {
      GLint length;
      glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
      GLchar* log = new char[length + 1];
      glGetShaderInfoLog(shaderID, length, &length, &log[0]);
      fprintf(stderr, "%s", log);
      return false;
   }
   return true;
}


bool checkShaderProgramLinkStatus(GLuint programID) 
{
   GLint status;
   glGetProgramiv(programID, GL_LINK_STATUS, &status);
   if(status == GL_FALSE) {
      GLint length;
      glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
      GLchar* log = new char[length + 1];
      glGetProgramInfoLog(programID, length, &length, &log[0]);
      fprintf(stderr, "%s", log);
      return false;   
   }
   return true;
}


int main()
{
   /* window dimensions */
   const GLuint WIDTH = 800, HEIGHT = 600;

   /*                                                                        */
   /* initialization and set-up                                              */
   /*                                                                        */
   /* initialization of GLFW */
   glfwSetErrorCallback(errorCallback);
   if (glfwInit() != GLFW_TRUE) {
      fprintf(stderr, "Cannot initialize GLFW\n");
      exit(EXIT_FAILURE);
   }

   /* set some GLFW options: we require OpenGL 3.3 (or more recent) context */
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

   /* create GFLW window (monitor in windowed mode), do not share resources */
   GLFWwindow* myWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Demo", 
                                           NULL, NULL);
   if (myWindow == NULL) {
      fprintf(stderr, "Cannot open GLFW window\n");
      exit(EXIT_FAILURE);
   }
   glfwMakeContextCurrent(myWindow);

   /* initialization of GLEW */
   glewExperimental = GL_TRUE;
   GLenum glewStatus = glewInit();
   if (glewStatus != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(glewStatus));
      exit(EXIT_FAILURE);
   }
   
   if (!GLEW_VERSION_2_0) {
      fprintf(stderr, "Error: GPU does not support GLEW 2.0\n");
      exit(EXIT_FAILURE);
   }

   /*                                                                        */
   /* define and allocate graphics objects and resources: quad               */
   /*                                                                        */
   GLfloat vtx[] = {
      -1.0f,-1.0f,-1.0f,   0.0f, 0.0f, /* bottom */
       1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
      -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
       1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
       1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,   0.0f, 0.0f, /* top */
      -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
       1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
       1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
      -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
       1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,   1.0f, 0.0f, /* font */
       1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
      -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
       1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
       1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
      -1.0f,-1.0f,-1.0f,   0.0f, 0.0f, /* back */
      -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
       1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
       1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
      -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
       1.0f, 1.0f,-1.0f,   1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,   0.0f, 1.0f, /* left */
      -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
      -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
      -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
       1.0f,-1.0f, 1.0f,   1.0f, 1.0f, /* right */
       1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
       1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
       1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
       1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
       1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };

   /* create and bind one Vertex Array Object */ 
   GLuint myVAO;
   glGenVertexArrays(1, &myVAO);
   glBindVertexArray(myVAO);

   /* generate and bind one Vertex Buffer Object */
   GLuint myVBO;
   glGenBuffers(1, &myVBO);
   glBindBuffer(GL_ARRAY_BUFFER, myVBO);

   /* copy the vertex data to it */
   glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STATIC_DRAW);

   /* OpenGL settings */
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   /* define and compile the vertex shader */
   const char* vertexShaderSource = GLSL(   
      in vec3 position;
      in vec2 textureCoordIn;
      uniform mat4 proj;
      uniform mat4 view;
      uniform mat4 anim;
      out vec2 textureCoordOut;
      void main() {
         textureCoordOut = 1.0 - textureCoordIn;
         gl_Position = proj * view * anim * vec4(position, 1.0);
      }
   );
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   /* check whether the vertex shader compiled without an error */
   if (!checkShaderCompileStatus(vertexShader)) {
      fprintf(stderr, "Vertex shader did not compile\n");
      exit(EXIT_FAILURE);
   }

   /* define and compile the fragment shader */
   const char* fragmentShaderSource = GLSL(
      in vec2 textureCoordOut;
      out vec4 outColor;
      uniform sampler2D textureData;
      void main() {
         outColor = texture(textureData, textureCoordOut);
      }
   );
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   /* check whether the fragment shader compiled without an error */
   if (!checkShaderCompileStatus(fragmentShader)) {
      fprintf(stderr, "Fragment shader did not compile\n");
      exit(EXIT_FAILURE);
   }

   /* create a shader program by linking the vertex and fragment shader */
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glBindFragDataLocation(shaderProgram, 0, "outColor");
   glLinkProgram(shaderProgram);

   /* check whether the shader program linked without an error */
   if (!checkShaderProgramLinkStatus(shaderProgram)) {
      fprintf(stderr, "Shader did not link\n");
      exit(EXIT_FAILURE);
   }

   /* make the shader program active */
   glUseProgram(shaderProgram);

   /* define how the input is organized */
   const char* attributeName;
   attributeName = "position";
   GLint posAttrib = glGetAttribLocation(shaderProgram, attributeName);
   if (posAttrib == -1) {
      fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
      exit(EXIT_FAILURE);
   }
   glEnableVertexAttribArray(posAttrib);
   glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 
                         5 * sizeof(GLfloat), 0);

   attributeName = "textureCoordIn";
   GLint texAttrib = glGetAttribLocation(shaderProgram, attributeName);
   if (texAttrib == -1) {
      fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
      exit(EXIT_FAILURE);
   }
   glEnableVertexAttribArray(texAttrib);
   glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 
                         5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

   /* load texture image */
   GLint texWidth, texHeight;
   GLint channels;
   unsigned char* texImage = SOIL_load_image("../img/katze.png", 
                                             &texWidth, &texHeight, &channels, 
                                             SOIL_LOAD_RGB);
   if (texImage == NULL) {
      fprintf(stderr, "Image file could not be loaded\n");
      exit(EXIT_FAILURE);
   }

   /* generate texture */
   GLuint textureID;
   glActiveTexture(GL_TEXTURE0);
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_2D, textureID);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, texImage);
   SOIL_free_image_data(texImage);

   /* set texture parameters */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   /* define a view transformation */
   glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));

   /* define a  projection transformation */
   glm::mat4 proj = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 10.0f);

   /* define a transformation matrix for the animation */
   glm::mat4 anim = glm::mat4(1.0f);

   /* bind uniforms and pass data to the shader program */
   const char* uniformName;
   uniformName = "textureData";
   GLint uniformTex = glGetUniformLocation(shaderProgram, uniformName);
   if (uniformTex == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
   }
   glUniform1i(uniformTex, 0);

   uniformName = "view";
   GLint uniformView = glGetUniformLocation(shaderProgram, uniformName);
   if (uniformView == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
   }
   glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

   uniformName = "proj";
   GLint uniformProj = glGetUniformLocation(shaderProgram, "proj");
   if (uniformProj == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
   }
   glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));

   uniformName = "anim";
   GLint uniformAnim = glGetUniformLocation(shaderProgram, uniformName); 
   if (uniformAnim == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
   }
   glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));

   /* register callback functions */ 
   glfwSetKeyCallback(myWindow, keyCallback);

   /*                                                                        */
   /* event-handling and rendering loop                                      */
   /*                                                                        */
   while (!glfwWindowShouldClose(myWindow)) {
      /* set the window background to black */
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* make the object spin around */ 
      //anim = glm::rotate(anim, 0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
      GLint rnd = rand() % 3;
      if (rnd == 0) {
         anim = glm::rotate(anim, glm::radians(0.5f), 
                            glm::vec3(0.0f, 0.0f, 1.0f));
      }
      else if (rnd == 1) {
         anim = glm::rotate(anim, glm::radians(0.5f), 
                            glm::vec3(0.0f, 1.0f, 0.0f));
      }
      else if (rnd == 2) {
         anim = glm::rotate(anim, glm::radians(0.5f), 
                            glm::vec3(1.0f, 0.0f, 0.0f));
      }
      glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));

      /* draw the VAO */
      glDrawArrays(GL_TRIANGLES, 0, 36);
      
      /* Swap buffers */
      glfwSwapBuffers(myWindow);

      /* poll events */
      glfwPollEvents();
   }

   /*                                                                        */
   /* clean-up and release resources                                         */
   /*                                                                        */
   glDeleteTextures(1, &textureID);
   
   glUseProgram(0);
   glDetachShader(shaderProgram, vertexShader);
   glDetachShader(shaderProgram, fragmentShader);
   glDeleteShader(fragmentShader);
   glDeleteShader(vertexShader);
   glDeleteProgram(shaderProgram);
   
   glDeleteBuffers(1, &myVBO);
   
   glDeleteVertexArrays(1, &myVAO);


   /*                                                                        */
   /* termination of GLFW                                                    */
   /*                                                                        */
   glfwTerminate();

   exit(EXIT_SUCCESS);
}
