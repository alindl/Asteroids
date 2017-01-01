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

#define GLM_FORCE_RADIANS
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
   /* define and allocate graphics objects and resources: quad consisting of */
   /* four triangles in the plane z=1                                        */
   /*                                                                        */
   float vtx[] = {
      -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, /* lower-left corner  */
       0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, /* lower-right corner */
       0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, /* upper-right corner */
      -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, /* upper-left corner  */
       0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f  /* center             */
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

   /* generate and bind one Index Buffer Object */
   GLuint myIBO;
   glGenBuffers(1, &myIBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);

   /* copy the index data to it */
   GLuint idx[] = {
      0, 1, 4,
      1, 2, 4,
      2, 3, 4,
      3, 0, 4
   };
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, 
                GL_STATIC_DRAW);

   /* define and compile the vertex shader */
   const char* vertexShaderSource = GLSL(   
      in vec3 position;
      in vec3 colorVtxIn;
      uniform mat4 model;
      out vec3 colorVtxOut;
      void main() {
         colorVtxOut = colorVtxIn;
         gl_Position = model * vec4(position, 1.0);
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
      in vec3 colorVtxOut;
      out vec4 outColor;
      void main() {
         outColor = vec4(colorVtxOut, 1.0f);
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
   glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 
                         6 * sizeof(GLfloat), 0);

   attributeName = "colorVtxIn";
   GLint colAttrib = glGetAttribLocation(shaderProgram, attributeName);
   if (colAttrib == -1) {
      fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
   }
   glEnableVertexAttribArray(colAttrib);
   glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 
                         6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

   /* define a model transformation: we rotate the object by  45 degrees */
   glm::mat4 model = glm::mat4(1.0f);
   model = glm::rotate(model, glm::radians(45.0f), 
                       glm::vec3(0.0f, 0.0f, 1.0f));

   /* bind uniforms and pass data to the shader program */
   const char* uniformName;
   uniformName = "model";
   /* pass the model matrix to the shader program */ 
   GLint uniformModel = glGetUniformLocation(shaderProgram, uniformName);
   if (uniformModel == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
   }
   glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

   /* register callback functions */ 
   glfwSetKeyCallback(myWindow, keyCallback);

   /*                                                                        */
   /* event-handling and rendering loop                                      */
   /*                                                                        */
   while (!glfwWindowShouldClose(myWindow)) {
      /* set the window background to black */
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      /* draw the quad: four triangles with a total of twelve vertices */
      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
      
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
   
   glDeleteBuffers(1, &myIBO);
   glDeleteBuffers(1, &myVBO);
   
   glDeleteVertexArrays(1, &myVAO);

   /*                                                                        */
   /* termination of GLFW                                                    */
   /*                                                                        */
   glfwTerminate();

   exit(EXIT_SUCCESS);
}
