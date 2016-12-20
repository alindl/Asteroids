/* standard includes */
#include<stdio.h>
#include<stdlib.h>

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
#include <SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
#include<GLFW/glfw3.h>

#define GLSL(src) "#version 330 core\n" #src

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
   /* define and allocate graphics objects and resources                     */
   /*                                                                        */
   float vtx[] = {
      0.0f,  0.0f, /* x- and y-coords of 1st vertex */
      0.5f,  0.5f, /* x- and y-coords of 2nd vertex */
      0.5f, -0.5f  /* x- and y-coords of 3rd vertex */
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

   /* define and compile the vertex shader */
   const char* vertexShaderSource = GLSL(   
      in vec2 position;
      void main() {
         gl_Position = vec4(position, 0.0f, 1.0f);
      }
   );
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   /* check whether the vertex shader has compiled without an error */
   GLint status;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
   if (status != GL_TRUE) {
      fprintf(stderr, "Vertex shader did not compile\n");
      char vertexCompilerLog[512];
      glGetShaderInfoLog(vertexShader, 512, NULL, vertexCompilerLog);
      fprintf(stderr, "%s", vertexCompilerLog);
      exit(EXIT_FAILURE);
   }

   /* define and compile the fragment shader */
   const char* fragmentShaderSource = GLSL(
      out vec4 outColor;
      void main() {
         outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
      }
   );
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   /* check whether the fragment shader has compiled without an error */
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
   if (status != GL_TRUE) {
      fprintf(stderr, "Fragment shader did not compile\n");
      char fragmentCompilerLog[512];
      glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentCompilerLog);
      fprintf(stderr, "%s", fragmentCompilerLog);
      exit(EXIT_FAILURE);
   }

   /* create a shader program by linking the vertex and fragment shader */
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glBindFragDataLocation(shaderProgram, 0, "outColor");
   glLinkProgram(shaderProgram);

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
   glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 
                         2 * sizeof(GLfloat), 0);

   /* register callback functions */ 
   glfwSetKeyCallback(myWindow, keyCallback);

   /*                                                                        */
   /* event-handling and rendering loop                                      */
   /*                                                                        */
   while (!glfwWindowShouldClose(myWindow)) {
      /* set the window background to black */
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      /* draw the triangle */
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      /* Swap buffers */
      glfwSwapBuffers(myWindow);

      /* poll events */
      glfwPollEvents();
   }

   /*                                                                        */
   /* clean-up and release resources                                         */
   /*                                                                        */
   glDeleteProgram(shaderProgram);
   glDeleteShader(fragmentShader);
   glDeleteShader(vertexShader);
   
   glDeleteBuffers(1, &myVBO);
   
   glDeleteVertexArrays(1, &myVAO);

   /*                                                                        */
   /* termination of GLFW                                                    */
   /*                                                                        */
   glfwTerminate();

   exit(EXIT_SUCCESS);
}
