//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "includes/shader.h"
#include "includes/camera.h"
#include "includes/basic_camera.h"
#include "includes/pointLight.h"
#include "includes/directionLight.h"
#include "includes/sphere.h"
#include "includes/spotLight.h"
#include "includes/cube.h"
#include "includes/stb_image.h"
#include "includes/cylinder.h"
#include "includes/curve.h"


#include <iostream>
#include <random>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
float run_speed = 0.004f;
//initial values

float gun_x=0.0f;
float gun_y = 1.0f;
float gun_z= -7.0f;

float bullet_x;
float bullet_y;
float slope_Y;
float slope_X;

float balloon_x=1.5f;
float balloon_y = 6.0f;
float balloon_z;
float bz = 0.0f;

float balloon_start = 0.0f;
float balloon_end = 6.0f;

float up_angle = 0.0f;
float right_angle = 0.0f;

bool first_block = true;
bool balloon = true;

int score = 0;
int remaining_time = 0;
int first_time = 1;


vector<float> bullet_points = {
-0.0550, 0.3250, 5.1000,
-0.1550, 0.3350, 5.1000,
-0.2750, 0.3400, 5.1000,
-0.4300, 0.3500, 5.1000,
-0.3900, 0.4400, 5.1000,
-0.3050, 0.5300, 5.1000,
-0.2900, 0.6800, 5.1000,
-0.2900, 0.8300, 5.1000,
-0.2900, 0.9800, 5.1000,
-0.2900, 1.1250, 5.1000,
-0.2900, 1.2200, 5.1000,
-0.2950, 1.3550, 5.1000,
-0.2950, 1.4850, 5.1000,
-0.2750, 1.6400, 5.1000,
-0.2250, 1.8050, 5.1000,
-0.1600, 1.8600, 5.1000,
-0.0950, 1.9550, 5.1000,
-0.0400, 1.9800, 5.1000,
};

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  5.50f,  0.0f),
    glm::vec3(1.5f,  5.5f,  10.0f),
    glm::vec3(-1.5f,  5.5f,  0.0f),
    glm::vec3(-1.5f,  5.5f,  10.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.25f, 0.25f, 0.25f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.25f, 0.25f, 0.25f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.25f, 0.25f, 0.25f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.25f, 0.25f, 0.25f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);





// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


class Balloon
{
public:
    float speed;
    float x=0.0,y=0.0,z=0.0;
    Sphere bb = Sphere();
};

void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Balloon b[])
{
    float baseHeight = 0.1;
    float width = 10;
    float length = 15;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, 0.3);


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;


    //wall
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.0f, 5.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.1f, 5.0f, 0.1f));
    //model = alTogether * translateMatrix * scaleMatrix;
    //lightingShader.setMat4("model", model);
    //drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);


    //gun
    Cylinder gun = Cylinder();
    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x, gun_y, gun_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
    glm::mat4 rotUp = glm::rotate(identityMatrix, glm::radians(up_angle-90.0f), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 0.0, 1.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    gun.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x-0.025, gun_y, gun_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle - 90.0f), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 0.0, 1.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    gun.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x-0.05, gun_y - 0.05, gun_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.07f, 0.35f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 1.0, 0.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.075, gun_y+0.025, gun_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.3f, 0.1f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle-180.0f), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(-right_angle), glm::vec3(0.0, 1.0, 0.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.8, 0.9);
    float bx = 0.0, by = 0.0;

    //shoot
    if (shoot)
    {
        bz += 0.05f;
        bx = slope_X * bz;
        by = slope_Y * bz;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(bullet_x + bx, bullet_y + by, bullet_z + bz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.20f));
        model = alTogether * translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);

        //bullet_curve.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));

        float bux = bullet_x + bx, buy = bullet_y + by, buz = bullet_z + bz;
        for (int i = 0; i < 2; i++)
        {
            if (buy < b[i].y) {
                float dis_y = b[i].y - buy;
                if (dis_y > 0.6) continue;
                float need_dis = 0.6 - dis_y;
                if (abs(b[i].x - bux) <= need_dis && abs(b[i].z - buz) <= need_dis) {
                    shoot = false;
                    bullet_z = -5.2f;
                    bz = 0.0;
                    std::random_device rd;  // Seed for the random number engine
                    std::mt19937 gen(rd());  // Mersenne Twister engine
                    std::uniform_real_distribution<float> dis(4.0f, 6.0f);
                    b[i].z = dis(gen);
                    std::uniform_real_distribution<float> dis2(-4, 4);
                    b[i].x = dis2(gen);
                    std::uniform_real_distribution<float> dis3(0.001, 0.003);
                    b[i].speed = dis3(gen);
                    b[i].y = balloon_start;
                    score += 5;
                }
            }
            else {
                float distance = sqrt((b[i].x - bux) * (b[i].x - bux) + (b[i].y - buy) * (b[i].y - buy) + (b[i].z - buz) * (b[i].z - buz));
                if (distance <= 0.6)
                {
                    shoot = false;
                    bullet_z = -5.2f;
                    bz = 0.0;
                    std::random_device rd;  // Seed for the random number engine
                    std::mt19937 gen(rd());  // Mersenne Twister engine
                    std::uniform_real_distribution<float> dis(4.0f, 6.0f);
                    b[i].z = dis(gen);
                    std::uniform_real_distribution<float> dis2(-4, 4);
                    b[i].x = dis2(gen);
                    std::uniform_real_distribution<float> dis3(0.001, 0.003);
                    b[i].speed = dis3(gen);
                    b[i].y = balloon_start;
                    score += 5;
                }
            }
        }
    }

    


    if (bullet_z + bz > 7.0f)
    {
        shoot = false;
        bullet_z = -5.2f;
        bz = 0.0;
    }





}


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Balloon Shooting", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "images/floor.jpg";
    string specularMapPath = diffuseMapPath;

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube floor = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "images/wall.jpg";
    specularMapPath = diffuseMapPath;

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "images/black.jpg";
    specularMapPath = diffuseMapPath;

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube table = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "images/game_over.png";
    specularMapPath = diffuseMapPath;

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube game_over_screen = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "images/start.png";
    specularMapPath = diffuseMapPath;

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube start_screen = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube score_blocks[10];

    diffuseMapPath = "images/start.png";
    specularMapPath = diffuseMapPath;

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve  bullet_curve = Curve(bullet_points, diffMap, specMap, 5);

   for (int i = 0; i < 10; i++) {
        diffuseMapPath = "images/digit-"+ to_string(i) + ".png";
        specularMapPath = diffuseMapPath;

        diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        score_blocks[i] = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    }

    string diffuseMapPathnum = "images/0.png";
    string specularMapPathnum = "images/0.png";
    unsigned int diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube zero = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Sphere display_balloon = Sphere();

    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(4.0f, 7.0f);
    float randomValue = dis(gen);
    balloon_z = randomValue;

    Balloon b[2];
    for (int i=0;i<2;i++)
    {
        //cout << "here" << endl;

        b[i].y = balloon_start;
        std::random_device rd;  // Seed for the random number engine
        std::mt19937 gen(rd());  // Mersenne Twister engine
        std::uniform_real_distribution<float> dis(4.0f, 6.0f);
        b[i].z = dis(gen);
        std::uniform_real_distribution<float> dis2(-4.0, 4.0);
        b[i].x = dis2(gen);
        std::uniform_real_distribution<float> dis3(0.001, 0.003);
        b[i].speed = dis3(gen);
    }

    //cout << camera.Position[0] << " " << camera.Position[1] << " " << camera.Position[2];
    camera.Position[2] = -11;
    camera.Position[1] += 1;
    int count = 0;
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera.Position = glm::vec3(0.0f, 1.0f, -7.5f);


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

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
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        
        if(remaining_time > 0)
            func(cubeVAO, lightingShader, model,b);


        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //balloon_y -= 0.001f;
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        //translateMatrix = glm::translate(model, glm::vec3(balloon_x, balloon_y, balloon_z));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
        //modelForSphere = translateMatrix * scaleMatrix;
        //sphere.drawSphere(lightingShader, modelForSphere);


        //if (balloon_y <= -1.0f)
        //{
        //    balloon_y = 6.0f;
        //    std::random_device rd;  // Seed for the random number engine
        //    std::mt19937 gen(rd());  // Mersenne Twister engine
        //    std::uniform_real_distribution<float> dis(4.0f, 6.0f);
        //    balloon_z = dis(gen);
        //    std::uniform_real_distribution<float> dis2(-1.0f, 1.0f);
        //    balloon_x = dis2(gen);
        //}

        //balloons
        if (remaining_time > 0) {
            for (int i = 0; i < 2; i++)
            {
                b[i].y += b[i].speed;
                translateMatrix = glm::translate(model, glm::vec3(b[i].x, b[i].y, b[i].z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
                modelForSphere = translateMatrix * scaleMatrix;
                b[i].bb.drawSphere(lightingShader, modelForSphere);

                if (b[i].y >= balloon_end)
                {
                    b[i].y = balloon_start;
                    std::random_device rd;  // Seed for the random number engine
                    std::mt19937 gen(rd());  // Mersenne Twister engine
                    std::uniform_real_distribution<float> dis(4.0f, 6.0f);
                    b[i].z = dis(gen);
                    std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
                    b[i].x = dis2(gen);
                    std::uniform_real_distribution<float> dis3(0.001, 0.003);
                    b[i].speed = dis3(gen);
                }
            }
            //display balloon

            translateMatrix = glm::translate(model, glm::vec3(-4.0f, 2.0f, 0.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
            modelForSphere = translateMatrix * scaleMatrix;
            display_balloon.drawSphere(lightingShader, modelForSphere);
        }

        


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }


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
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);




        if (remaining_time == 0) {
            if (first_time) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-40.0, -40.0f, -30.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(100.0f, 50.0f, 50.0f));
                glm::mat4 modelForGameOver = model * translateMatrix * scaleMatrix;
                start_screen.drawCubeWithTexture(lightingShaderWithTexture, modelForGameOver);
            }
            else {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-40.0, -40.0f, -30.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(100.0f, 50.0f, 50.0f));
                glm::mat4 modelForGameOver = model * translateMatrix * scaleMatrix;
                game_over_screen.drawCubeWithTexture(lightingShaderWithTexture, modelForGameOver);
                int A[4] = {};
                int val = score;
                int ind = 3;
                while (val > 0) {
                    A[ind] = val % 10;
                    val /= 10;
                    ind--;
                }


                for (int i = 0; i < 4; i++) {
                    translateMatrix = glm::translate(identityMatrix, glm::vec3(15 - 5 * i, -20.5f, -30.5f));
                    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 5.0f, 0.05f));
                    glm::mat4 modelForScore1 = model * translateMatrix * scaleMatrix;
                    score_blocks[A[i]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);
                }
            }
        }
        else {

            
            count++;
            if (count == 800) {
                remaining_time--;
                count = 0;
            }
            //floor
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.5f, -0.1f, -8.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.1f, 20.0f));
            glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            floor.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);



            //left wall
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.3f, -0.1f, -8.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 10.0f, 20.0f));
            modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            wall.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);



            //right wall
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.5f, -0.1f, -8.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 10.0f, 20.0f));
            modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            wall.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);



            //back wall
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.5f, -0.1f, 12.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 10.0f, 0.2f));
            modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            wall.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

            //table
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.5f, -0.2f, -8.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 1.0f, 1.0f));
            modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            wall.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);


            // bullet

            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0, -24.0f, -88.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            //glm::mat4 rotate_bul = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 0.0));
            glm::mat4 modelBullet = model * translateMatrix * scaleMatrix;
            bullet_curve.draw(lightingShader, modelBullet, glm::vec3(1.0f, 0.0f, 0.0f));


            //score board
            int A[4] = {};
            int val = score;
            int ind = 3;
            while (val > 0) {
                A[ind] = val % 10;
                val /= 10;
                ind--;
            }


            for (int i = 0; i < 4; i++) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5 - 5 * i, 0.0f, 5.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 5.0f, 0.05f));
                glm::mat4 modelForScore1 = model * translateMatrix * scaleMatrix;
                score_blocks[A[i]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);
            }

            //score board
            A[3] = {};
            val = remaining_time;
            ind = 2;
            while (val > 0) {
                A[ind] = val % 10;
                val /= 10;
                ind--;
            }


            for (int i = 0; i < 3; i++) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(24 - 5 * i, 0.0f, 5.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 5.0f, 0.05f));
                glm::mat4 modelForScore1 = model * translateMatrix * scaleMatrix;
                score_blocks[A[i]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);
            }
        }

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

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

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //gun_y += 0.003f;

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //gun_y -= 0.003f;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // gun_x += 0.003f;
        camera.ProcessKeyboard(LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // gun_x -= 0.003f;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        // gun_x -= 0.003f;
        if (remaining_time == 0) {
            remaining_time = 10;
            score = 0;
            first_time = 0;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!shoot)
        {
            shoot = true;
            bullet_x = gun_x;
            bullet_y = gun_y;
            slope_X = tan(right_angle * 3.1416 / 180.0);
            slope_Y = -1*tan(up_angle * 3.1416 / 180.0);
        }
            
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        up_angle += 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        up_angle -= 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        right_angle += 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        right_angle -= 0.05f;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
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
