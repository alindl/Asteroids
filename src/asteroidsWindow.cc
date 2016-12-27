/* standard includes */
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <string>

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
#include <SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
#include<GLFW/glfw3.h>
GLFWwindow* window;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>

#define GLSL(src) "#version 330 core\n" #src

/*                                                                           */
/* GLFW callback functions for event handling                                */
/*                                                                           */
static void errorCallback(int error, const char* description)
{
   fprintf(stderr, "GLFW error %d: %s\n", error, description);
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Demo",
                                           NULL, NULL);
   if (window == NULL) {
      fprintf(stderr, "Cannot open GLFW window\n");
      exit(EXIT_FAILURE);
   }
   glfwMakeContextCurrent(window);

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
   /* event-handling and rendering loop                                      */
   /*                                                                        */
   while (!glfwWindowShouldClose(window)) {

      /* Swap buffers */
      glfwSwapBuffers(window);

      /* poll events */
      glfwPollEvents();

      /* close window upon hitting the escape or Q key */
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
         glfwSetWindowShouldClose(window, GL_TRUE);
      if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
               glfwSetWindowShouldClose(window, GL_TRUE);
   }

   /*                                                                        */
   /* termination of GLFW                                                    */
   /*                                                                        */


   glfwTerminate();

   exit(EXIT_SUCCESS);
}
