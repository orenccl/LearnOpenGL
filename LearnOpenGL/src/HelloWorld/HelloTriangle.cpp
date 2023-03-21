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

    // shaser code
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
        "layout (location = 1) in vec3 aColor;\n" // the color variable has attribute position 1
        "out vec3 ourColor;\n" // output a color to the fragment shader
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n" // directly give a vec3 to vec4's constructor
        "   ourColor = aColor;\n" // set ourColor to the input color we got from the vertex data
        "}\0";

    const char* fragmentShaderSource1 = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor; \n" // the input variable from the vertex shader (same name and same type)
        "void main()\n"
        "{\n"
        "   FragColor = vec4(ourColor, 1.0);\n"
        "}\n\0";

    const char* fragmentShaderSource2 = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 ourColor;\n" // set this variable in the OpenGL code.
        "void main()\n"
        "{\n"
        "   FragColor = ourColor;\n"
        "}\n\0";

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

        Shader ourShader("../Shaders/Shader.vs", "../Shaders/Shader1.fs");

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // check for shader compile errors
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // orange fragment shader
        unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
        glCompileShader(fragmentShader1);

        // check for shader compile errors
        glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // yellow fragment shader
        unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
        glCompileShader(fragmentShader2);

        // check for shader compile errors
        glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link orange shaders
        unsigned int shaderProgram1 = glCreateProgram();
        glAttachShader(shaderProgram1, vertexShader);
        glAttachShader(shaderProgram1, fragmentShader1);
        glLinkProgram(shaderProgram1);

        // check for linking error
        glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // link yellow shaders
        unsigned int shaderProgram2 = glCreateProgram();
        glAttachShader(shaderProgram2, vertexShader);
        glAttachShader(shaderProgram2, fragmentShader2);
        glLinkProgram(shaderProgram2);

        // check for linking error
        glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete shader after link to program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader1);
        glDeleteShader(fragmentShader2);

        // Set up vertex and indices data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float firstTriangle[] = {
            // positions            // colors
             -0.25f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // top
            -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // bottom left
             0.0f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f  // bottom right
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(VAOs[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
        // registered VBO as the vertex attribute's bound vertex buffer object
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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

            // draw our first triangle
            glUseProgram(shaderProgram1);
            // only have a single VAO there's no need to bind it every time, but do so to keep things a bit more organized
            glBindVertexArray(VAOs[0]);
            // draw rectangle
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // be sure to activate the shader
            glUseProgram(shaderProgram2);

            // update the uniform color
            float timeValue = glfwGetTime();
            float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
            int vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
            glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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
        glDeleteProgram(shaderProgram1);
        glDeleteProgram(shaderProgram2);

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
