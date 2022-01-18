#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iomanip>
#include <iostream>
using namespace std;
using namespace glm;

vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

vec3 prism = vec3(0.0f, 0.0f, 0.0f);

mat4 model;
mat4 view;
mat4 projection;

bool is_rotating = false;
bool is_revolving = false;

float cameraSpeed;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        is_rotating = !is_rotating;

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        is_revolving = !is_revolving;

    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        prism += normalize(cameraUp) * 0.1f;
        model = translate(model, normalize(cameraUp) * 0.1f);
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        prism += normalize(cameraUp) * -0.1f;
        model = translate(model, normalize(cameraUp) * -0.1f);
    }

    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        prism += normalize(cross(cameraFront, cameraUp)) * -0.1f;
        model = translate(model, normalize(cross(cameraFront, cameraUp)) * -0.1f);
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        prism += normalize(cross(cameraFront, cameraUp)) * 0.1f;
        model = translate(model, normalize(cross(cameraFront, cameraUp)) * 0.1f);
    }

    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        prism += normalize(cameraFront) * -0.1f;
        model = translate(model, normalize(cameraFront) * -0.1f);
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        prism += normalize(cameraFront) * 0.1f;
        model = translate(model, normalize(cameraFront) * 0.1f);
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraSpeed * up;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraSpeed * up;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
        cameraPos = vec3(2.0f, 0.0f, 3.0f);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        cameraPos = vec3(0.0f, 2.0f, 2.0f);
}

int main(int argc, char *argv[]) {
    int n;

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <NUM_VERTICES>" << endl;
        return 1;
    }

    n = atoi(argv[1]);

    if (n < 3) {
        cout << "Sides should be at least 3!" << endl;
        return 0;
    }

    float theta = 2 * M_PI / (float)n;
    int NUM_VERTICES = n * 9 * 2 * 2 * 2; // N vertices, 2 triangles at z = 0 and z = 1, 2 triangles to connect each side, color for each vertex
    float vertices[NUM_VERTICES];

    cout << setprecision(2) << fixed;

    int idx = 0;

    bool connecting_color_flag = true;
    bool main_color_flag = true;

    for (int i = 0; i < NUM_VERTICES; i += 72, idx++) {
        float color1[3] = {0.746094, 0.378906, 0.414062};
        float color2[3] = {0.535156, 0.8125, 0.4375};
        float color3[3] = {0.439, 0.537, 0.816};
        float color4[3] = {0.917969, 0.792969, 0.542969};

        // float color1[3] = {rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX};
        // float color2[3] = {rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX};
        // float color3[3] = {rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX};
        // float color4[3] = {rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX};

        // first triangle (color1)
        vertices[i + 0] = 0;
        vertices[i + 1] = 0;
        vertices[i + 2] = -0.5;

        // cout << "[" << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << "]" << endl;

        vertices[i + 3] = color1[0];
        vertices[i + 4] = color1[1];
        vertices[i + 5] = color1[2];

        vertices[i + 6] = cos(theta * idx);
        vertices[i + 7] = sin(theta * idx);
        vertices[i + 8] = -0.5;

        // cout << "[" << vertices[i + 6] << ", " << vertices[i + 7] << ", " << vertices[i + 8] << "]" << endl;

        vertices[i + 9] = color1[0];
        vertices[i + 10] = color1[1];
        vertices[i + 11] = color1[2];

        vertices[i + 12] = cos(theta * (idx + 1));
        vertices[i + 13] = sin(theta * (idx + 1));
        vertices[i + 14] = -0.5;

        // cout << "[" << vertices[i + 12] << ", " << vertices[i + 13] << ", " << vertices[i + 14] << "]" << endl;

        vertices[i + 15] = color1[0];
        vertices[i + 16] = color1[1];
        vertices[i + 17] = color1[2];

        // 2nd triangle (opposite triangle) (color1)
        vertices[i + 18] = 0;
        vertices[i + 19] = 0;
        vertices[i + 20] = 0.5;

        // cout << "[" << vertices[i + 18] << ", " << vertices[i + 19] << ", " << vertices[i + 20] << "]" << endl;

        vertices[i + 21] = color2[0];
        vertices[i + 22] = color2[1];
        vertices[i + 23] = color2[2];

        vertices[i + 24] = cos(theta * idx);
        vertices[i + 25] = sin(theta * idx);
        vertices[i + 26] = 0.5;

        // cout << "[" << vertices[i + 24] << ", " << vertices[i + 25] << ", " << vertices[i + 26] << "]" << endl;

        vertices[i + 27] = color2[0];
        vertices[i + 28] = color2[1];
        vertices[i + 29] = color2[2];

        vertices[i + 30] = cos(theta * (idx + 1));
        vertices[i + 31] = sin(theta * (idx + 1));
        vertices[i + 32] = 0.5;

        // cout << "[" << vertices[i + 30] << ", " << vertices[i + 31] << ", " << vertices[i + 32] << "]" << endl;

        vertices[i + 33] = color2[0];
        vertices[i + 34] = color2[1];
        vertices[i + 35] = color2[2];

        // 3rd triangle (connecting triangle 1)
        vertices[i + 36] = cos(theta * idx);
        vertices[i + 37] = sin(theta * idx);
        vertices[i + 38] = -0.5;

        // cout << "[" << vertices[i + 36] << ", " << vertices[i + 37] << ", " << vertices[i + 38] << "]" << endl;

        vertices[i + 39] = connecting_color_flag ? color3[0] : color4[0];
        vertices[i + 40] = connecting_color_flag ? color3[1] : color4[1];
        vertices[i + 41] = connecting_color_flag ? color3[2] : color4[2];

        vertices[i + 42] = cos(theta * (idx + 1));
        vertices[i + 43] = sin(theta * (idx + 1));
        vertices[i + 44] = -0.5;

        // cout << "[" << vertices[i + 42] << ", " << vertices[i + 43] << ", " << vertices[i + 44] << "]" << endl;

        vertices[i + 45] = connecting_color_flag ? color3[0] : color4[0];
        vertices[i + 46] = connecting_color_flag ? color3[1] : color4[1];
        vertices[i + 47] = connecting_color_flag ? color3[2] : color4[2];

        vertices[i + 48] = cos(theta * (idx + 1));
        vertices[i + 49] = sin(theta * (idx + 1));
        vertices[i + 50] = 0.5;

        // cout << "[" << vertices[i + 48] << ", " << vertices[i + 49] << ", " << vertices[i + 50] << "]" << endl;

        vertices[i + 51] = connecting_color_flag ? color3[0] : color4[0];
        vertices[i + 52] = connecting_color_flag ? color3[1] : color4[1];
        vertices[i + 53] = connecting_color_flag ? color3[2] : color4[2];

        // 4th triangle (connecting triangle 2)
        vertices[i + 54] = cos(theta * (idx + 1));
        vertices[i + 55] = sin(theta * (idx + 1));
        vertices[i + 56] = 0.5;

        // cout << "[" << vertices[i + 54] << ", " << vertices[i + 55] << ", " << vertices[i + 56] << "]" << endl;

        vertices[i + 57] = connecting_color_flag ? color3[0] : color4[0];
        vertices[i + 58] = connecting_color_flag ? color3[1] : color4[1];
        vertices[i + 59] = connecting_color_flag ? color3[2] : color4[2];

        vertices[i + 60] = cos(theta * idx);
        vertices[i + 61] = sin(theta * idx);
        vertices[i + 62] = 0.5;

        // cout << "[" << vertices[i + 60] << ", " << vertices[i + 61] << ", " << vertices[i + 62] << "]" << endl;

        vertices[i + 63] = connecting_color_flag ? color3[0] : color4[0];
        vertices[i + 64] = connecting_color_flag ? color3[1] : color4[1];
        vertices[i + 65] = connecting_color_flag ? color3[2] : color4[2];

        vertices[i + 66] = cos(theta * idx);
        vertices[i + 67] = sin(theta * idx);
        vertices[i + 68] = -0.5;

        // cout << "[" << vertices[i + 66] << ", " << vertices[i + 67] << ", " << vertices[i + 68] << "]" << endl;

        vertices[i + 69] = connecting_color_flag ? color3[0] : color4[0];
        vertices[i + 70] = connecting_color_flag ? color3[1] : color4[1];
        vertices[i + 71] = connecting_color_flag ? color3[2] : color4[2];

        connecting_color_flag = !connecting_color_flag;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "3D Polygons", NULL, NULL);

    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    Shader shaders("vertex.shader", "fragment.shader");

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vec3 cameraDirection = normalize(cameraPos - cameraTarget);
    vec3 cameraRight = normalize(cross(up, cameraDirection));
    vec3 cameraUp = cross(cameraDirection, cameraRight);

    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, key_callback);

    model = mat4(1.0f);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.180f, 0.203f, 0.250f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);

        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(120.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shaders.use();

        shaders.setMat4("model", model);
        shaders.setMat4("view", view);
        shaders.setMat4("projection", projection);

        if (is_rotating) {
            model = rotate(model, radians(1.0f), vec3(1.0, 0.0, 0.0));
        }

        if (is_revolving) {
            vec3 cameraRight = normalize(cross(cameraFront, up));
            cameraPos += cameraRight * 0.1f;
            cameraFront = prism - cameraPos;
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        glfwSwapBuffers(window); // Swap the buffers
        glfwPollEvents();        // Check for events
    }

    glDeleteVertexArrays(1, &VAO); // Delete the VAO
    glDeleteBuffers(1, &VBO);      // Delete the VBO

    glfwTerminate(); // Terminate GLFW, clearing any resources allocated by GLFW.
    return 0;
}