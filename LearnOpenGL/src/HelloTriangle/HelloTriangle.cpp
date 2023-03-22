#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "HelloTriangle.h"
#include "../Shader.h"

namespace HelloTriangle
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    int Main()
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

        Shader firstShader("shaders/VertexShaders/Shader.vs", "shaders/FragmentShaders/Shader1.fs");
        Shader secondShader("shaders/VertexShaders/Shader.vs", "shaders/FragmentShaders/Shader2.fs");

        // Set up vertex and indices data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float firstTriangle[] = {
            // positions            // color start      // color middle     // color end
             -0.25f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,// top
            -0.5f, -0.3f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,// bottom left
             0.0f, -0.3f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f // bottom right
        };

        float secondTriangle[] = {
            0.25f,  0.5f, 0.0f,  // top
            0.5f, -0.5f, 0.0f,  // bottom left
            0.0f, -0.5f, 0.0f,  // bottom right
        };

        unsigned int VAOs[2], VBOs[2];
        glGenVertexArrays(2, VAOs);
        glGenBuffers(2, VBOs);

        // bind the Vertex Array Oject first, then bind and set vertex buffer(s), and then configure vertex attribute(s)
        glBindVertexArray(VAOs[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // color attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // color attribute
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glBindVertexArray(VAOs[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
        // registered VBO as the vertex attribute's bound vertex buffer object
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        float gradientStopTime = -glfwGetTime();
        float gradientValue = 0;
        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            float time = glfwGetTime();

            // draw our first triangle
            firstShader.use();

            // stop 0.4 secs
            if (time - gradientStopTime > 0.4) {
                // interval 0.02
                gradientValue += 0.02;
                // cyclic from 0 to 3
                float cyclicNumber = fmod(gradientValue, 3);
                firstShader.setFloat("gradientValue", cyclicNumber);

                // stop when cyclicNumber is interger
                if (cyclicNumber - floor(cyclicNumber) < 0.02) {
                    gradientStopTime = time;
                }
            }
            // only have a single VAO there's no need to bind it every time, but do so to keep things a bit more organized
            glBindVertexArray(VAOs[0]);
            // draw rectangle
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // be sure to activate the shader
            secondShader.use();
            // update the uniform color
            float sinValue = (sin(time) / 2.0f) + 0.5f;
            secondShader.setFloat("greenValue", sinValue);
            secondShader.setFloat("xOffset", sinValue / 2);
            // only have a single VAO there's no need to bind it every time, but do so to keep things a bit more organized
            glBindVertexArray(VAOs[1]);
            // draw rectangle
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(2, VAOs);
        glDeleteBuffers(2, VBOs);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}
