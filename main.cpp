//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "Sphere2.h"
#include "cube.h"
#include "stb_image.h"
#include "Pyramid.h"

#include "curve.h"
#include <iostream>

using namespace std;

bool isOn[] = { 1,1,1 };

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b, float er, float eg, float eb);
void pointlight(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float r, float g, float b, float er, float eg, float eb);
void directionlight(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float r, float g, float b, float er, float eg, float eb);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void spotlight(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float r, float g, float b, float er, float eg, float eb);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

glm::mat4 transformation(float tx, float ty, float tz, float sx, float sy, float sz, float rX, float rY, float rZ) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rZ), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;
    return model;
}


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(32.0f,  2.5f,  -80.0f),
    glm::vec3(-4.675f,  -0.6f,  -2.1f),
    glm::vec3(-0.5f,  5.5f,  -10.0f),
    /*glm::vec3(1.50f,  1.50f,  0.0f)*/
    
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.4f, 0.4f, 0.4f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.3f, 0.3f, 0.3f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);

PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.6f, 0.6f, 0.6f,     // ambient
    0.3f, 0.3f, 0.3f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
//PointLight pointlight4(
//
//    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    4       // light number
//);


// light settings
bool pointLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string twodiffuseMapPath = "2.png";
    string twospecularMapPath = "";

    unsigned int twodiffMap = loadTexture(twodiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int twospecMap = loadTexture(twospecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube twocube = Cube(twodiffMap, twospecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string onediffuseMapPath = "1.png";
    string onespecularMapPath = "";

    unsigned int onediffMap = loadTexture(onediffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int onespecMap = loadTexture(onespecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube onecube = Cube(onediffMap, onespecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string citydiffuseMapPath = "city.jpg";
    string cityspecularMapPath = "";

    unsigned int citydiffMap = loadTexture(citydiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int cityspecMap = loadTexture(cityspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube citycube = Cube(citydiffMap, cityspecMap, 32.0f, 0.0f, 0.0f, 5.0f, 1.0f);

    string desertdiffuseMapPath = "desert.jpg";
    string desertspecularMapPath = "";

    unsigned int desertdiffMap = loadTexture(desertdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int desertspecMap = loadTexture(desertspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube desertcube = Cube(desertdiffMap, desertspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string mountaindiffuseMapPath = "mountain.jpg";
    string mountainspecularMapPath = "";

    unsigned int mountaindiffMap = loadTexture(mountaindiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int mountainspecMap = loadTexture(mountainspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube mountaincube = Cube(mountaindiffMap, mountainspecMap, 32.0f, 0.0f, 0.0f, 5.0f, 1.0f);

    string garagediffuseMapPath = "garage.jpg";
    string garagespecularMapPath = "";

    unsigned int garagediffMap = loadTexture(garagediffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int garagespecMap = loadTexture(garagespecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube garagecube = Cube(garagediffMap, garagespecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string seadiffuseMapPath = "sea.bmp";
    string seaspecularMapPath = "";

    unsigned int seadiffMap = loadTexture(seadiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int seaspecMap = loadTexture(seaspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seacube = Cube(seadiffMap, seaspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 5.0f);

    string fifthdiffuseMapPath = "fifth.png";
    string fifthspecularMapPath = "";

    unsigned int fifthdiffMap = loadTexture(fifthdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int fifthspecMap = loadTexture(fifthspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fifthcube = Cube(fifthdiffMap, fifthspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string fourthdiffuseMapPath = "fourth.png";
    string fourthspecularMapPath = "";

    unsigned int fourthdiffMap = loadTexture(fourthdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int fourthspecMap = loadTexture(fourthspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fourthcube = Cube(fourthdiffMap, fourthspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string thirddiffuseMapPath = "third.png";
    string thirdspecularMapPath = "";

    unsigned int thirddiffMap = loadTexture(thirddiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int thirdspecMap = loadTexture(thirdspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube thirdcube = Cube(thirddiffMap, thirdspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string seconddiffuseMapPath = "second.png";
    string secondspecularMapPath = "";

    unsigned int seconddiffMap = loadTexture(seconddiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int secondspecMap = loadTexture(secondspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube secondcube = Cube(seconddiffMap, secondspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string firstdiffuseMapPath = "First.png";
    string firstspecularMapPath = "";

    unsigned int firstdiffMap = loadTexture(firstdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int firstspecMap = loadTexture(firstspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube firstcube = Cube(firstdiffMap, firstspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string sanddiffuseMapPath = "pyramid.jpg";
    string sandspecularMapPath = "";

    unsigned int sanddiffMap = loadTexture(sanddiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sandspecMap = loadTexture(sandspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sandcube = Cube(sanddiffMap, sandspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string wooddiffuseMapPath = "wood.bmp";
    string woodspecularMapPath = "";

    unsigned int wooddiffMap = loadTexture(wooddiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int woodspecMap = loadTexture(woodspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube woodcube = Cube(wooddiffMap, woodspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string streetdiffuseMapPath = "z-direction.bmp";
    string streetspecularMapPath = "";

    unsigned int streetdiffMap = loadTexture(streetdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int streetspecMap = loadTexture(streetspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube streetCube = Cube(streetdiffMap, streetspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string street2diffuseMapPath = "x-direction.bmp";
    string street2specularMapPath = "";

    unsigned int street2diffMap = loadTexture(street2diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int street2specMap = loadTexture(street2specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube street2Cube = Cube(street2diffMap, street2specMap, 32.0f, 0.0f, 0.0f, 5.0f, 1.0f);

    string building1diffuseMapPath = "bu.bmp";
    string building1specularMapPath = "";

    unsigned int building1diffMap = loadTexture(building1diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int building1specMap = loadTexture(building1specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube building1cube = Cube(building1diffMap, building1specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string building2diffuseMapPath = "bu3.bmp";
    string building2specularMapPath = "";

    unsigned int building2diffMap = loadTexture(building2diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int building2specMap = loadTexture(building2specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube building2cube = Cube(building2diffMap, building2specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string building3diffuseMapPath = "left-front building-front.bmp";
    string building3specularMapPath = "";

    unsigned int building3diffMap = loadTexture(building3diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int building3specMap = loadTexture(building3specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube building3cube = Cube(building3diffMap, building3specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string building4diffuseMapPath = "tunnel.jpg";
    string building4specularMapPath = "";

    unsigned int building4diffMap = loadTexture(building4diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int building4specMap = loadTexture(building4specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube building4cube = Cube(building4diffMap, building4specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string dayskydiffuseMapPath = "sky.bmp";
    string dayskyspecularMapPath = "";

    unsigned int dayskydiffMap = loadTexture(dayskydiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int dayskyspecMap = loadTexture(dayskyspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dayskycube = Cube(dayskydiffMap, dayskyspecMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string grassdiffuseMapPath = "grass.png";
    string grassspecularMapPath = "";

    unsigned int grassdiffMap = loadTexture(grassdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int grassspecMap = loadTexture(grassspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grasscube = Cube(grassdiffMap, grassspecMap, 2.0f, 0.0f, 0.0f, 5.0f, 5.0f);

    string footpathdiffuseMapPath = "footpath.jpg";
    string footpathspecularMapPath = "";

    unsigned int footpathdiffMap = loadTexture(footpathdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int footpathspecMap = loadTexture(footpathspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube footpathcube = Cube(footpathdiffMap, footpathspecMap, 2.0f, 0.0f, 0.0f, 1.0f, 5.0f);

    string footpath2diffuseMapPath = "footpath2.jpg";
    string footpath2specularMapPath = "";

    unsigned int footpath2diffMap = loadTexture(footpath2diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int footpath2specMap = loadTexture(footpath2specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube footpath2cube = Cube(footpath2diffMap, footpath2specMap, 2.0f, 0.0f, 0.0f, 1.0f, 2.0f);

    vector<float> hillPoints = {
        -0.9700, 0.1400, 5.1000,
        -0.9250, 0.3100, 5.1000,
        -0.8750, 0.3900, 5.1000,
        -0.8300, 0.4350, 5.1000,
        -0.8000, 0.5300, 5.1000,
        -0.7650, 0.6400, 5.1000,
        -0.6400, 0.8200, 5.1000,
        -0.6000, 0.9350, 5.1000,
        -0.6050, 1.0200, 5.1000,
        -0.6050, 1.0850, 5.1000,
        -0.5400, 1.1550, 5.1000,
        -0.4900, 1.2000, 5.1000,
        -0.4150, 1.2950, 5.1000,
        -0.3850, 1.4550, 5.1000,
        -0.3700, 1.5450, 5.1000,
        -0.3450, 1.6950, 5.1000,
        -0.3100, 1.7750, 5.1000,
        -0.3150, 1.9050, 5.1000,
        -0.2000, 2.0150, 5.1000,
        -0.1850, 2.0250, 5.1000,
        -0.0950, 2.1400, 5.1000,
        -0.0350, 2.2150, 5.1000,
    };
    string hilldiffuseMapPath = "hill.jpg";
    string hillspecularMapPath = "";
    unsigned int hilldiffMap = loadTexture(hilldiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int hillspecMap = loadTexture(hillspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve hill = Curve(hillPoints, hilldiffMap, hillspecMap, 0.0);

    vector<float>fishPoints = {
        -0.0100, 1.9950, 5.1000,
        -0.0550, 1.9800, 5.1000,
        -0.0950, 1.9350, 5.1000,
        -0.1500, 1.8250, 5.1000,
        -0.2250, 1.5900, 5.1000,
        -0.2550, 1.3450, 5.1000,
        -0.2050, 1.1700, 5.1000,
        -0.1400, 1.0050, 5.1000,
        -0.0400, 0.8600, 5.1000,
        0.0400, 0.7300, 5.1000,
        0.1300, 0.6350, 5.1000,
        0.2400, 0.5050, 5.1000,
    };

    string fishdiffuseMapPath = "fish.jpeg";
    string fishspecularMapPath = "";
    unsigned int fishdiffMap = loadTexture(fishdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int fishspecMap = loadTexture(fishspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve fish = Curve(fishPoints, fishdiffMap, fishspecMap, 0.0);

    string leavesdiffuseMapPath = "leaves.png";
    string leavesspecularMapPath = "";

    unsigned int leavesdiffMap = loadTexture(leavesdiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int leavesspecMap = loadTexture(leavesspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve leaves = Curve(hillPoints, leavesdiffMap, leavesspecMap, 0.0);

    vector<float> treeBasePoints = {
        -0.2450, 1.8600, 5.1000,
        -0.2450, 1.7600, 5.1000,
        -0.1900, 1.6300, 5.1000,
        -0.1900, 1.5150, 5.1000,
        -0.2050, 1.4350, 5.1000,
        -0.2050, 1.3500, 5.1000,
        -0.1950, 1.2500, 5.1000,
        -0.1900, 1.1550, 5.1000,
        -0.1900, 1.0400, 5.1000,
        -0.2050, 0.9800, 5.1000,
        -0.2350, 0.8700, 5.1000,
        -0.2700, 0.7750, 5.1000,
    };
    string treeBasediffuseMapPath = "treebase.png";
    string treeBasespecularMapPath = "";
    unsigned int treeBasediffMap = loadTexture(treeBasediffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int treeBasespecMap = loadTexture(treeBasespecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve treeBase = Curve(treeBasePoints, treeBasediffMap, treeBasespecMap, 2.0);

    vector<float> tyrePoints = {
        -0.0200, 1.4400, 5.1000,
        -0.1450, 1.4550, 5.1000,
        -0.2150, 1.4550, 5.1000,
        -0.2850, 1.4550, 5.1000,
        -0.2900, 1.3950, 5.1000,
        -0.2900, 1.3200, 5.1000,
        -0.2850, 1.2800, 5.1000,
        -0.2700, 1.2700, 5.1000,
        -0.1750, 1.2550, 5.1000,
        -0.1050, 1.2550, 5.1000,
        -0.0650, 1.2600, 5.1000,
    };
    string tyrediffuseMapPath = "";
    string tyrespecularMapPath = "";
    unsigned int tyrediffMap = loadTexture(tyrediffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int tyrespecMap = loadTexture(tyrespecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve tyre = Curve(tyrePoints, tyrediffMap, tyrespecMap, 0);

    string sundiffuseMapPath = "moon.png";
    string sunspecularMapPath = "";
    unsigned int sundiffMap = loadTexture(sundiffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sunspecMap = loadTexture(sunspecularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Sphere2 sun = Sphere2(0.5f, 144, 72, 
        glm::vec3(1.0f, 0.0f, 0.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f), 
        glm::vec3(0.5f, 0.5f, 0.5f), 
        32.0f, sundiffMap, sunspecMap,
        0.0f, 0.0f, 1.0f, 1.0f);

    Pyramid pyramid = Pyramid("pyramid.jpg");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //ourShader.use();
    //lightingShader.use();
    double prevTime = glfwGetTime();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;



        double currentTime = glfwGetTime();

        // Calculate the elapsed time since the last frame
        double deltaTime = currentTime - prevTime;
        int sec = deltaTime;



        // Print the elapsed time for demonstration purposes
        std::cout << "Elapsed Time: " << sec << " seconds" << std::endl;
        if (sec > 300) break;

        // Update the previous frame time
        //prevTime = currentTime;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        //// point light 1
        pointlight1.setUpPointLight(lightingShader);
        //// point light 2
        //pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        /*pointlight4.setUpPointLight(lightingShader);*/

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 1, 1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //for (unsigned int i = 0; i < 4; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //    drawCube(cubeVAO, lightingShader, model, 1, 1, 1, 1, 1, 1);
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        //drawCube(cubeVAO, lightingShader, model, 1, 1, 1, 1, 1, 1);
        if(isOn[1]) pointlight(cubeVAO, lightingShader, model, 1, 1, 1, 1, 1, 1);
        else pointlight(cubeVAO, lightingShader, model, 1,1,1,0,0,0);

        /*if (isOn[2]) directionlight(cubeVAO, lightingShader, model, 1, 1, 1, 1, 1, 1);
        else directionlight(cubeVAO, lightingShader, model, 1, 1, 1, 0, 0, 0);*/

        if (isOn[0]) spotlight(cubeVAO, lightingShader, model, 1, 1, 1, 1, 1, 1);
        else spotlight(cubeVAO, lightingShader, model, 1, 1, 1, 0, 0, 0);

        //bed(cubeVAO, lightingShader, model);
        //street(cubeVAO, lightingShader, model);
        
        

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        
        lightingShaderWithTexture.setMat4("projection", projection);

        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);

        glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, pointLightPositions[2]);
        sun.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);

        //sky
        glm::mat4 model2 = glm::mat4(1.0f);
        glm::mat4 translate = glm::mat4(1.0f);
        glm::mat4 translate2 = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(8, 9, 0));
        scale = glm::scale(model2, glm::vec3(68, 0.05, 96));
        translate = glm::translate(model2, glm::vec3(-0.5, -60, -1));
        model2 = model * translate2 * scale * translate;
        dayskycube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //sky left
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-26, -3, 0));
        scale = glm::scale(model2, glm::vec3(0.1, 9, 96));

        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        mountaincube.drawCubeWithTexture(lightingShaderWithTexture, model2);


        //sky front
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(5, -3, -96));
        scale = glm::scale(model2, glm::vec3(70, 9, 0.1));

        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        desertcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //sky right
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(42, -3, 0));
        scale = glm::scale(model2, glm::vec3(0.1, 9, 96));

        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        citycube.drawCubeWithTexture(lightingShaderWithTexture, model2);
        

        //footpath left
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-5, -3, -20));
        scale = glm::scale(model2, glm::vec3(2, 0.1, 8));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        footpathcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //footpath left
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-5, -3, -48));
        scale = glm::scale(model2, glm::vec3(2, 0.1, 8));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        footpathcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        for (int z = 0; z <= 56; z+=28) {
            for (int x = 0; x <= 32; x+=16) {
                //street main
                model2 = glm::mat4(1.0f);
                translate2 = glm::translate(model2, glm::vec3(0+x, 0, 0-z));
                scale = glm::scale(model2, glm::vec3(8, 0.05, 20));
                translate = glm::translate(model2, glm::vec3(-0.5, -60, -1));
                model2 = model * translate2 * scale * translate;
                streetCube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                

                //footpath left
                model2 = glm::mat4(1.0f);

                translate2 = glm::translate(model2, glm::vec3(-5+x, -3, 0-z));
                scale = glm::scale(model2, glm::vec3(2, 0.1, 20));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate;
                footpathcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                if (!(x == 32 and z == 28)) {
                    //footpath right
                    model2 = glm::mat4(1.0f);

                    translate2 = glm::translate(model2, glm::vec3(7 + x, -3, 0 - z));
                    scale = glm::scale(model2, glm::vec3(6, 0.1, 20));
                    translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                    model2 = model * translate2 * scale * translate;
                    footpath2cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                    //building1
                    model2 = glm::mat4(1.0f);

                    translate2 = glm::translate(model2, glm::vec3(7 + x, -3, -1 - z));
                    scale = glm::scale(model2, glm::vec3(4, 8, 4));
                    translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                    model2 = model * translate2 * scale * translate;
                    building1cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                    //building2
                    model2 = glm::mat4(1.0f);

                    translate2 = glm::translate(model2, glm::vec3(7 + x, -3, -7 - z));
                    scale = glm::scale(model2, glm::vec3(4, 8, 4));
                    translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                    model2 = model * translate2 * scale * translate;
                    building2cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                    //building3
                    model2 = glm::mat4(1.0f);

                    translate2 = glm::translate(model2, glm::vec3(7 + x, -3, -13 - z));
                    scale = glm::scale(model2, glm::vec3(4, 8, 4));
                    translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                    model2 = model * translate2 * scale * translate;
                    building3cube.drawCubeWithTexture(lightingShaderWithTexture, model2);
                }

                

                //street light
                model2 = glm::mat4(1.0f);

                translate2 = glm::translate(model2, glm::vec3(-4.5+x, -3, -2-z));
                scale = glm::scale(model2, glm::vec3(0.1, 2.4, 0.1));
                translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
                model2 = model * translate2 * scale * translate;
                woodcube.drawCubeWithTexture(lightingShaderWithTexture, model2);
            }
        }

        //pond side up
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(7 + 32, -3, 0 - 20));
        scale = glm::scale(model2, glm::vec3(6, 0.3, 0.5));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        drawCube(cubeVAO,lightingShader, model2, 1,1,1,0,0,0);

        //pond side down
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(7 + 32, -3, 0 - 55.5));
        scale = glm::scale(model2, glm::vec3(6, 0.3, 0.5));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        //pond side left
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(7 + 29.25, -3, 0 - 20.5));
        scale = glm::scale(model2, glm::vec3(0.5, 0.3, 35));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        //pond side right
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(7 + 34.75, -3, 0 - 20.5));
        scale = glm::scale(model2, glm::vec3(0.5, 0.3, 35));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        //pond
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(7 + 32, -3, 0 - 20.5));
        scale = glm::scale(model2, glm::vec3(5, 0.1, 35));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        seacube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //fish
        model2 = glm::mat4(1.0f);
        rotateYMatrix = glm::rotate(model2, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        translate2 = glm::translate(model2, glm::vec3(7 + 32, -3, 0 - 24));
        scale = glm::scale(model2, glm::vec3(3, 3, 4));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate * rotateYMatrix;
        fish.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));

        //street light2
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-3, -0.6, -2 - 28));
        scale = glm::scale(model2, glm::vec3(2, 2, 0.1));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        firstcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //street light3
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-3, -0.6, -2 - 56));
        scale = glm::scale(model2, glm::vec3(2, 2, 0.1));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        secondcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //street light4
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-3+16, -0.6, -2 - 56));
        scale = glm::scale(model2, glm::vec3(2, 2, 0.1));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        thirdcube.drawCubeWithTexture(lightingShaderWithTexture, model2);
        
        //street light5
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-3 + 16, -0.6, -2 - 28));
        scale = glm::scale(model2, glm::vec3(2, 2, 0.1));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        fourthcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //street light6
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-3 + 32, -0.6, -2 - 28));
        scale = glm::scale(model2, glm::vec3(2, 2, 0.1));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        fifthcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //street light7
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-4.45 + 32, -0.6, -2 - 56));
        scale = glm::scale(model2, glm::vec3(0.2, 0.2, 0.2));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        if(isOn[1]) drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 1, 1, 1);
        else drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        //street light8
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-4.45 + 16, -0.6, -2));
        scale = glm::scale(model2, glm::vec3(0.2, 0.2, 0.2));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        if (isOn[1]) drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 1, 1, 1);
        else drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        //street light9
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-4.45 + 32, -0.6, -2));
        scale = glm::scale(model2, glm::vec3(0.2, 0.2, 0.2));
        translate = glm::translate(model2, glm::vec3(-1.25, 0, -0.5));
        model2 = model * translate2 * scale * translate;
        if (isOn[1]) drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 1, 1, 1);
        else drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        for (int z = 0; z <= 28; z += 28) {
            //x-street
            model2 = glm::mat4(1.0f);

            translate2 = glm::translate(model2, glm::vec3(16, -3, -20-z));
            scale = glm::scale(model2, glm::vec3(40, 0.05, 8));
            translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
            model2 = model * translate2 * scale * translate;
            street2Cube.drawCubeWithTexture(lightingShaderWithTexture, model2);
        }

        for (int z = 0; z <= 56; z += 28) {

            if (z == 56) {
                //sand
                model2 = glm::mat4(1.0f);

                translate2 = glm::translate(model2, glm::vec3(-16, -3, 0 - z));
                scale = glm::scale(model2, glm::vec3(20, 0.1, 20));

                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate;
                sandcube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                //pyramid1
                model2 = glm::mat4(1.0f);

                translate2 = glm::translate(model2, glm::vec3(-7, -3.35, 0 - z));
                scale = glm::scale(model2, glm::vec3(9, 9, 9));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate;
                pyramid.draw(lightingShaderWithTexture, model2);

                //pyramid1
                model2 = glm::mat4(1.0f);

                translate2 = glm::translate(model2, glm::vec3(-16, -3.35, 0 - z));
                scale = glm::scale(model2, glm::vec3(7, 7, 7));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate;
                pyramid.draw(lightingShaderWithTexture, model2);
            }
            else {
                //grass
                model2 = glm::mat4(1.0f);

                translate2 = glm::translate(model2, glm::vec3(-16, -3, 0 - z));
                scale = glm::scale(model2, glm::vec3(20, 0.1, 28));
                
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate;
                grasscube.drawCubeWithTexture(lightingShaderWithTexture, model2);

                //hill
                model2 = glm::mat4(1.0f);

                rotateYMatrix = glm::rotate(model2, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                translate2 = glm::translate(model2, glm::vec3(-16, -3.35, 0 - z));
                scale = glm::scale(model2, glm::vec3(4, 3, 4));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate * rotateYMatrix;
                hill.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));

                //hill2
                model2 = glm::mat4(1.0f);

                rotateYMatrix = glm::rotate(model2, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                translate2 = glm::translate(model2, glm::vec3(-16, -4, -7 - z));
                scale = glm::scale(model2, glm::vec3(3, 4, 3));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate * rotateYMatrix;
                hill.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));

                //hill3
                model2 = glm::mat4(1.0f);

                rotateYMatrix = glm::rotate(model2, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                translate2 = glm::translate(model2, glm::vec3(-16, -3.5, -12 - z));
                scale = glm::scale(model2, glm::vec3(4, 4, 4));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = model * translate2 * scale * translate * rotateYMatrix;
                hill.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));

                for (int i = 0; i <= 15; i += 5) {
                    //tree base
                    model2 = glm::mat4(1.0f);

                    rotateYMatrix = glm::rotate(model2, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    translate2 = glm::translate(model2, glm::vec3(-8, -5.7, 0 - i - z));
                    scale = glm::scale(model2, glm::vec3(2, 3.5, 2));
                    translate = glm::translate(model2, glm::vec3(-0.5, 0, -2));
                    model2 = model * translate2 * scale * translate * rotateYMatrix;
                    treeBase.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));

                    //leaves
                    model2 = glm::mat4(1.0f);

                    rotateYMatrix = glm::rotate(model2, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    translate2 = glm::translate(model2, glm::vec3(-8, 0, -2 - i - z));
                    scale = glm::scale(model2, glm::vec3(2, 2, 2));
                    translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                    model2 = model * translate2 * scale * translate * rotateYMatrix;
                    leaves.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));
                }
            }
            
        }

        for (int x = 0; x <= 20; x += 20) {
            //sand
            model2 = glm::mat4(1.0f);

            translate2 = glm::translate(model2, glm::vec3(-16+x, -3, 0 - 76));
            scale = glm::scale(model2, glm::vec3(20, 0.1, 20));

            translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
            model2 = model * translate2 * scale * translate;
            sandcube.drawCubeWithTexture(lightingShaderWithTexture, model2);
        }

        //treasureland
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-16 + 40, -3, 0 - 76));
        scale = glm::scale(model2, glm::vec3(20, 0.05, 20));

        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        footpath2cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasureland
        model2 = glm::mat4(1.0f);

        translate2 = glm::translate(model2, glm::vec3(-16 + 53.75, -3, 0 - 76));
        scale = glm::scale(model2, glm::vec3(7.5, 0.05, 20));

        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        footpath2cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasure
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 30, -3, 0 - 76 - 8));
        scale = glm::scale(model2, glm::vec3(2, 2, 2));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasure
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 32, -3, 0 - 76 - 8));
        scale = glm::scale(model2, glm::vec3(2, 2, 2));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasure
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 34, -3, 0 - 76 - 8));
        scale = glm::scale(model2, glm::vec3(2, 2, 2));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasure
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 31, -3+2, 0 - 76 - 8));
        scale = glm::scale(model2, glm::vec3(2, 2, 2));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasure
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 33, -3 + 2, 0 - 76 - 8));
        scale = glm::scale(model2, glm::vec3(2, 2, 2));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasure
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 32, -3 + 4, 0 - 76 - 8));
        scale = glm::scale(model2, glm::vec3(2, 2, 2));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasureland building back
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 32, -3, 0 - 76 -10));
        scale = glm::scale(model2, glm::vec3(10, 7, 0.1));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        building4cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasureland building top
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 32, -3+6, 0 - 76));
        scale = glm::scale(model2, glm::vec3(9, 1, 10));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        building4cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasureland building right
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 32+5, -3, 0 - 76));
        scale = glm::scale(model2, glm::vec3(1, 7, 10));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        building4cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //treasureland building left
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0 + 27, -3, 0 - 76));
        scale = glm::scale(model2, glm::vec3(1, 7, 10));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = model * translate2 * scale * translate;
        building4cube.drawCubeWithTexture(lightingShaderWithTexture, model2);

        //garage door
        glm::mat4 modelgarage = glm::mat4(1.0f);
        translate2 = glm::translate(modelgarage, glm::vec3(0 + 32, -3, 0 - 75.9));
        scale = glm::scale(modelgarage, glm::vec3(9, 6, 0.1));
        translate = glm::translate(modelgarage, glm::vec3(-0.5, 0, -1));
        modelgarage = transformation(0, 0+translate_Y, 0, 1, 1, 1, 0, 0, 0) * translate2 * scale * translate;
        garagecube.drawCubeWithTexture(lightingShaderWithTexture, modelgarage);

        
        

        camera.Position.z += translate_Z;
        camera.Position.x += translate_X;
        camera.Yaw -= rotateAngle_Y*10000;

        for (int x = 0; x <= 2; x+=2) {
            for (int z = 0; z <= 3; z+=3) {
                //tyre
                model2 = glm::mat4(1.0f);
                translate2 = glm::translate(model2, glm::vec3(2.75+x, -2.4, 1.5-z));
                rotateXMatrix = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                scale = glm::scale(model2, glm::vec3(2, 2, 2));
                translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
                model2 = transformation(0+ camera.Position.x, 0, -5+ camera.Position.z, 1, 1, 1, 0, rotateAngle_Y, 0) * translate2 * scale * translate * rotateXMatrix;
                tyre.draw(lightingShaderWithTexture, model2, glm::vec3(1.0f));
            }
        }

        //car body
        glm::mat4 modelcar = glm::mat4(1.0f);
        translate2 = glm::translate(modelcar, glm::vec3(0.025, -1.95, 1.5));
        scale = glm::scale(modelcar, glm::vec3(2.2, 0.5, 6));
        translate = glm::translate(modelcar, glm::vec3(-0.5, 0, -1));
        modelcar = transformation(0 + camera.Position.x, 0, -5 + camera.Position.z, 1, 1, 1, 0, rotateAngle_Y, 0) * translate2 * scale * translate;
        drawCube(cubeVAO, lightingShader, modelcar, 0.6, 0, 0, 0, 0, 0);

        //car top
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0.025, -1.45,0.5));
        scale = glm::scale(model2, glm::vec3(1.5, 0.8, 3));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = transformation(0 + camera.Position.x, 0, -5 + camera.Position.z, 1, 1, 1, 0, rotateAngle_Y, 0) * translate2 * scale * translate;
        drawCube(cubeVAO, lightingShader, model2, 1, 1, 1, 0, 0, 0);

        //car windshield back
        model2 = glm::mat4(1.0f);
        translate2 = glm::translate(model2, glm::vec3(0.025, -1.45, 0.51));
        scale = glm::scale(model2, glm::vec3(1.2, 0.65, 0.1));
        translate = glm::translate(model2, glm::vec3(-0.5, 0, -1));
        model2 = transformation(0 + camera.Position.x, 0, -5 + camera.Position.z, 1, 1, 1, 0, rotateAngle_Y, 0) * translate2 * scale * translate;
        drawCube(cubeVAO, lightingShader, model2, 0, 0, 0, 0, 0, 0);
        
        glm::vec3 garagePosition = glm::vec3(modelgarage[3]);
        glm::vec3 carPosition = glm::vec3(modelcar[3]);

        glm::mat4 modelc1 = glm::mat4(1.0f);
        translate2 = glm::translate(modelc1, glm::vec3(0, -3, -20));
        scale = glm::scale(modelc1, glm::vec3(2, 2, 2));
        translate = glm::translate(modelc1, glm::vec3(-0.5, 0, -1));
        modelc1 = model * translate2 * scale * translate;
        if (glm::distance(carPosition, glm::vec3(modelc1[3])) > 5 and !c1) {
            drawCube(cubeVAO, lightingShader, modelc1, 0.627, 0.125, 0.94, 0, 0, 0);
        }
        else c1 = 1;

        if (c1) {
            glm::mat4 modelc2 = glm::mat4(1.0f);
            translate2 = glm::translate(modelc2, glm::vec3(0, -3, -20 - 28));
            scale = glm::scale(modelc2, glm::vec3(2, 2, 2));
            translate = glm::translate(modelc2, glm::vec3(-0.5, 0, -1));
            modelc2 = model * translate2 * scale * translate;
            if (glm::distance(carPosition, glm::vec3(modelc2[3])) > 5 and !c2) {
                drawCube(cubeVAO, lightingShader, modelc2, 0.627, 0.125, 0.94, 0, 0, 0);
            }
            else c2 = 1;
        }

        if (c2) {
            glm::mat4 modelc3 = glm::mat4(1.0f);
            translate2 = glm::translate(modelc3, glm::vec3(0+16, -3, -20 - 28));
            scale = glm::scale(modelc3, glm::vec3(2, 2, 2));
            translate = glm::translate(modelc3, glm::vec3(-0.5, 0, -1));
            modelc3 = model * translate2 * scale * translate;
            if (glm::distance(carPosition, glm::vec3(modelc3[3])) > 5 and !c3) {
                drawCube(cubeVAO, lightingShader, modelc3, 0.627, 0.125, 0.94, 0, 0, 0);
            }
            else c3 = 1;
        }

        if (c3) {
            glm::mat4 modelc4 = glm::mat4(1.0f);
            translate2 = glm::translate(modelc4, glm::vec3(0 + 16, -3, -20-4));
            scale = glm::scale(modelc4, glm::vec3(2, 2, 2));
            translate = glm::translate(modelc4, glm::vec3(-0.5, 0, -1));
            modelc4 = model * translate2 * scale * translate;
            if (glm::distance(carPosition, glm::vec3(modelc4[3])) > 5 and !c4) {
                drawCube(cubeVAO, lightingShader, modelc4, 0.627, 0.125, 0.94, 0, 0, 0);
            }
            else c4 = 1;
        }

        if (c4) {
            glm::mat4 modelc5 = glm::mat4(1.0f);
            translate2 = glm::translate(modelc5, glm::vec3(0 + 16+16, -3, -20-4));
            scale = glm::scale(modelc5, glm::vec3(2, 2, 2));
            translate = glm::translate(modelc5, glm::vec3(-0.5, 0, -1));
            modelc5 = model * translate2 * scale * translate;
            if (glm::distance(carPosition, glm::vec3(modelc5[3])) > 5 and !c5) {
                drawCube(cubeVAO, lightingShader, modelc5, 0.627, 0.125, 0.94, 0, 0, 0);
            }
            else c5 = 1;
        }

        if (c5) {
            if (glm::distance(carPosition, garagePosition) < 10) {
                translate_Y += 0.01;
            }
        }
        
       
        
        
        
        
        
        // also draw the lamp object(s)
        /*ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);*/

        // we now draw as many light bulbs as we have point lights.
        //glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 2; i < 3; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //    ourShader.setMat4("model", model);
        //    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f, float er = 0.0f, float eg = 0.0f, float eb = 0.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setVec3("material.emissive", glm::vec3(er, eg, eb));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void pointlight(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float r = 1.0f, float g = 1.0f, float b = 1.0f, float er = 0.0f, float eg = 0.0f, float eb = 0.0f) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, pointLightPositions[1]);
    scale = glm::scale(model, glm::vec3(0.2f));
    translate = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = alTogether * translate2 * scale  * translate;
    drawCube(cubeVAO, lightingShader, model, r,g,b, er, eg, eb);
}

void directionlight(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float r = 1.0f, float g = 1.0f, float b = 1.0f, float er = 0.0f, float eg = 0.0f, float eb = 0.0f) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, pointLightPositions[2]);
    scale = glm::scale(model, glm::vec3(0.7f));
    translate = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, r, g, b, er, eg, eb);
}

void spotlight(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float r = 1.0f, float g = 1.0f, float b = 1.0f, float er = 0.0f, float eg = 0.0f, float eb = 0.0f) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, pointLightPositions[0]);
    scale = glm::scale(model, glm::vec3(0.2f));
    translate = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, r, g, b, er, eg, eb);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime*5);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime*5);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime*5);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime*5);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotateAngle_Y -= 0.2;
        translate_X += 0.00005;
        translate_Z += 0.00005;
    }
    else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        rotateAngle_Y += 0.2;
        translate_X -= 0.00005;
        translate_Z += 0.00005;
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotateAngle_Y -= 0.2;
        translate_Z += 0.00005;
        translate_X -= 0.00005;
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        rotateAngle_Y += 0.2;
        translate_Z += 0.00005;
        translate_X += 0.00005;
    }
    else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Z -= 0.0001;
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Z += 0.0001;
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotateAngle_Y -= 0.2;
        translate_X += 0.00005;
        translate_Z += 0.00005;
    }
    else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        rotateAngle_Y += 0.2;
        translate_X -= 0.00005;
        translate_Z += 0.00005;
    }
    

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }

    
    

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (isOn[0])
        {
            pointlight1.turnOff();
            isOn[0] = 0;
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            isOn[0] = 1;
            pointLightOn = !pointLightOn;
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (isOn[1])
        {
            
            pointlight2.turnOff();
            isOn[1] = 0;
            pointLightOn = !pointLightOn;
        }
        else
        {
            
            pointlight2.turnOn();
            isOn[1] = 1;
            pointLightOn = !pointLightOn;
        }
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (isOn[2])
        {
            
            pointlight3.turnOff();
            isOn[2] = 0;
            
        }
        else
        {
            
            pointlight3.turnOn();
            isOn[2] = 1;
            
        }
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
   
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
