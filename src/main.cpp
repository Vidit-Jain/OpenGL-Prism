#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <iostream>
#include "shader.h"
#include "camera.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
float rotation_prism = 0;
float rotation_camera = 0;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float delta = 0.0f, last = 0.0f;
glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 preset1 = glm::vec3(5.0f, 5.0f, 5.0f);
glm::vec3 preset2 = glm::vec3(0.0f, 0.0f, 3.0f);
int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Incorrect number of arguments passed\n";
        return 0;
    }
    int x = atoi(argv[1]);
    if (x <= 2) {
        cout << "Invalid number of sides entered for a polygon\n";
        return 0;
    }
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    Shader ourShader("../src/vertex.shader", "../src/fragment.shader");
    double theta = (double)360 / x;
    theta *= (double) M_PI / 180;
    glEnable(GL_DEPTH_TEST);
    double r = 0.5f;
    float vertices[72 * x];
    float randomColor[3];
    float randomColor2[3];
    for (int i = 0; i < 3; i++) {
        randomColor[i] = (rand() % 255) / 255.0f;
        randomColor2[i] = (rand() % 255) / 255.0f;
    }
    for (int i = 0; i < x; i++) {
        // Center vertex
        vertices[18 * (x + i)] = vertices[18 * i] = 0.0f;
        vertices[18 * (x + i) + 1] =  vertices[18 * i + 1] = 0.0f;
        vertices[18 * i + 2] = 0.5f;
        vertices[18 * (x + i) + 2] =  -0.5f;

        // Second vertex
        vertices[18 * (x + i) + 6] = vertices[18 * i + 6] = r * cos(i * theta);
        vertices[18 * (x + i) + 6 + 1] = vertices[18 * i + 6 + 1] = r * sin(i * theta);
        vertices[18 * i + 6 + 2] = 0.5f;
        vertices[18 * (x + i) + 6 + 2] = -0.5f;

        // Third vertex anticlockwise
        vertices[18 * (x + i) + 12] =  vertices[18 * i + 12] = r * cos((i + 1) * theta);
        vertices[18 * (x + i) + 12 + 1] = vertices[18 * i + 12 + 1] = r * sin((i + 1) * theta);
        vertices[18 * i + 12 + 2] = 0.5f;
        vertices[18 * (x + i) + 12 + 2] = -0.5f;

        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                vertices[18 * i + j * 6 + k + 3] = randomColor[k];
                vertices[18 * (x + i) + j * 6 + k + 3] = randomColor2[k];
            }
        }
    }

    for (int i = 0; i < x; i++) {
        float color[3];
        for (int j = 0; j < 3; j++) {
            color[j] = (rand() % 255) / 255.0f;
        }
        // Top and bottom vertices
        vertices[36 * (x + i) + 6] = vertices[36 * (x + i)] = r * cos(i * theta);
        vertices[36 * (x + i) + 6 + 1] = vertices[36 * (x + i) + 1] = r * sin(i * theta);
        vertices[36 * (x + i) + 2] = 0.5f;
        vertices[36 * (x + i) + 6 + 2] = -0.5f;

        vertices[36 * (x + i) + 12] = r * cos((i + 1)  * theta);
        vertices[36 * (x + i) + 12 + 1] = r * sin((i + 1)  * theta);
        vertices[36 * (x + i) + 12 + 2] = 0.5f;


        vertices[36 * (x + i) + 6 + 18] = vertices[36 * (x + i) + 18] = r * cos((i + 1) * theta);
        vertices[36 * (x + i) + 6 + 1 + 18] = vertices[36 * (x + i) + 1 + 18] = r * sin((i + 1) * theta);
        vertices[36 * (x + i) + 2 + 18] = 0.5f;
        vertices[36 * (x + i) + 6 + 2 + 18] = -0.5f;

        vertices[36 * (x + i) + 12 + 18] = r * cos(i  * theta);
        vertices[36 * (x + i) + 12 + 1 + 18] = r * sin(i  * theta);
        vertices[36 * (x + i) + 12 + 2 + 18] = -0.5f;

        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                vertices[36 * (x + i) + 6 * j + k + 3] = color[k];
                vertices[36 * (x + i) + 6 * j + k + 3 + 18] = color[k];
            }
        }
    }
    unsigned int indices[12 * x];
    for (int i = 0; i < 12 * x; i++) {
        indices[i] = i;
    }
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


//     uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
//    glm::mat4 view = glm::mat4(1.0f);

// note that we're translating the scene in the reverse direction of where we want to move
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        float current = glfwGetTime();
        delta = current - last;
        last = current;
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw our first triangle
//        glUseProgram(shaderProgram);
        ourShader.use();
        glm::mat4 trans = glm::mat4(1.0f);
        double sins = sin(glfwGetTime());
//        trans = glm::translate(trans, glm::vec3(0.0f, sins / 1.5, 0.0f));
        trans = glm::translate(trans, center);
        trans = glm::rotate(trans, (float)rotation_prism, glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
        const float radius = 10.0f;
        float camX = sin(rotation_camera) * radius;
        float camZ = cos(rotation_camera) * radius;
//        glm::mat4 view;
//        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 view = camera.GetViewMatrix(center);
        ourShader.setMat4("transform", trans);
        ourShader.setMat4("perspective", projection);
        ourShader.setMat4("view", view);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, sizeof (indices), GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
//    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotation_prism += 0.05;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        rotation_camera += 0.05;
    // Moving camera around
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, delta);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta);
    // Moving object
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        center += glm::vec3(0.0f, 0.0f, delta * 2.5f);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        center += glm::vec3(0.0f, 0.0f, -delta * 2.5f);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        center += glm::vec3(delta * 2.5f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        center += glm::vec3(-delta * 2.5f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        center += glm::vec3(0.0f, -delta * 2.5f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        center += glm::vec3(0.0f, delta * 2.5f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        camera.setPosition(preset1);
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        camera.setPosition(preset2);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

