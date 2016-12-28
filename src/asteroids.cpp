// Std. Includes
#include <string>
#include <algorithm>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>
//#include <learnopengl/filesystem.h>
using namespace std;

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void Do_Movement();
GLuint loadTexture(GLchar const * path);
GLuint loadCubemap(std::vector<std::string> faces);

// Camera
Camera camera(glm::vec3(0.0f, 100.0f, 155.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Asteroids 3D", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Setup and compile our shaders
//    Shader shader("shader.vs", "shader.frag");
//    Shader shader("cubemaps.vs", "cubemaps.frag");
    Shader skyboxShader("skybox.vs", "skybox.frag");
    Shader instanceShader("instanced_asteroids.vs", "instanced_asteroids.frag");
    Shader planetShader("planet.vs", "planet.frag");

    // Load models
//    Model ourModel("/home/panda/Downloads/Akira/akira.obj");
//    Model ourModel("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/nanosuit/nanosuit.obj");
//    Model ourModel("/home/panda/Downloads/mb8u5ripu5ts-Arc-170_ship/[.obj] (Sh3d adapted)/Arc170.obj");
//    Model ourModel("/home/panda/Downloads/Voyager NCC 74656/voyager.obj");
//    Model ourModel("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/rock/rock.obj");
    Model rock("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/rock/rock.obj");
    Model planet("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/planet/planet.obj");

#pragma region "object_initialization"

    // Set the object data (buffers, vertex attributes)
//    GLfloat cubeVertices[] = {
//        // Positions          // Normals
//        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
//        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
//        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
//        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
//        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
//        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
//
//        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//
//        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
//        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
//        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
//        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
//
//        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
//        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
//        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
//        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
//        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
//        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
//        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
//        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
//        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
//        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
//
//        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
//        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
//        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
//        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
//    };

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    // Setup cube VAO
//    GLuint cubeVAO, cubeVBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//    glBindVertexArray(0);

    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

#pragma endregion

    // Cubemap (Skybox)
    std::vector<std::string> faces;
    faces.push_back("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/skybox/right.png");
    faces.push_back("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/skybox/left.png");
    faces.push_back("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/skybox/top.png");
    faces.push_back("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/skybox/bottom.png");
    faces.push_back("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/skybox/back.png");
    faces.push_back("/home/panda/MEGA/Uni/5. Semester/CG/Asteroids/src/skybox/front.png");
    GLuint skyboxTexture = loadCubemap(faces);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 10000.0f);
    planetShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Also of instance shader
    instanceShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Generate a large list of semi-random model transformation matrices
    GLuint amount = 20000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed
    GLfloat radius = 150.0f;
    GLfloat offset = 25.0f;
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
      GLfloat scale = (rand() % 20) / 100.0f + 0.05;
      model = glm::scale(model, glm::vec3(scale));

      // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
      GLfloat rotAngle = (rand() % 360);
      model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

      // 4. Now add to list of matrices
      modelMatrices[i] = model;
    }

    // forward declare the buffer
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // Set transformation matrices as an instance vertex attribute (with divisor 1)
    // NOTE: We're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // Normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
//    for(GLuint i = 0; i < rock.meshes.size(); i++)
//    {
//      GLuint VAO = rock.meshes[i].VAO;
//      glBindVertexArray(VAO);
//      // Set attribute pointers for matrix (4 times vec4)
//      glEnableVertexAttribArray(3);
//      glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
//      glEnableVertexAttribArray(4);
//      glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
//      glEnableVertexAttribArray(5);
//      glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
//      glEnableVertexAttribArray(6);
//      glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
//
//      glVertexAttribDivisor(3, 1);
//      glVertexAttribDivisor(4, 1);
//      glVertexAttribDivisor(5, 1);
//      glVertexAttribDivisor(6, 1);
//
//      glBindVertexArray(0);
//    }

     // Draw in wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
      // Set frame time
      GLfloat currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // Check and call events
      glfwPollEvents();
      Do_Movement();

      // Clear the colorbuffer
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Add transformation matrices
      planetShader.Use();
      glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
      instanceShader.Use();
      glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

      // Draw Planet
      planetShader.Use();
      glm::mat4 model;
      model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
      model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
      glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
      planet.Draw(planetShader);

      // Draw meteorites
      instanceShader.Use();
      // NB: This could all be implemented as a method within the Model class, perhaps "DrawInstanced(const GLuint amount)"
      glActiveTexture(GL_TEXTURE0); // Activate proper texture unit before binding
      glUniform1i(glGetUniformLocation(instanceShader.Program, "texture_diffuse1"), 0); // Now set the sampler to the correct texture unit
      glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // Note we also made the textures_loaded vector public (instead of private) from the model class.
      for(GLuint i = 0; i < rock.meshes.size(); i++)
      {
        glBindVertexArray(rock.meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
      }

      // reset our texture binding
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 0);


//        ourModel.Draw(shader);
//        shader.Use();   // <-- Don't forget this one!
        // Transformation matrices
//        glm::mat4 model;

//        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
//        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
//        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glUniform3f(glGetUniformLocation(shader.Program, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

        // Draw the loaded model
//        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
//        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down




        // Cubes
//        glBindVertexArray(cubeVAO);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glBindVertexArray(0);

      // Draw skybox as last
      glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
      skyboxShader.Use();
      view = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // Remove any translation component of the view matrix
      glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


      glBindVertexArray(skyboxVAO);
      glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // Set depth function back to default

      // Swap the buffers
      glfwSwapBuffers(window);
    }

    delete[] modelMatrices;

    glfwTerminate();
    return 0;
}

GLuint loadCubemap(std::vector<std::string> faces)
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

// This function loads a texture from file. Note: texture loading functions like these are usually
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio).
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar const * path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input (*20 because we now we are faster)
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime*20);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime*20);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime*20);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime*20);
    if(keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(PITCHUP, deltaTime*20);
    if(keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(PITCHDOWN, deltaTime*20);
    if(keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(YAWLEFT, deltaTime*20);
    if(keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(YAWRIGHT, deltaTime*20);
    if(keys[GLFW_KEY_SPACE])
        camera.ProcessKeyboard(UP, deltaTime*20);
    if(keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(DOWN, deltaTime*20);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; 
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}	

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
//    for (GLuint i = 0;i < 100 && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS; i++)
//      camera.ProcessKeyboard(FORWARD, deltaTime*20);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

#pragma endregion
