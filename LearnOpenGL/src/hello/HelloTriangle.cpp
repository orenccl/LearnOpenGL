#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "HelloTriangle.h"

namespace HelloTriangle
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    // shaser code
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderOrangeSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    const char* fragmentShaderYellowSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
        unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderOrange, 1, &fragmentShaderOrangeSource, NULL);
        glCompileShader(fragmentShaderOrange);

        // check for shader compile errors
        glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // yellow fragment shader
        unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSource, NULL);
        glCompileShader(fragmentShaderYellow);

        // check for shader compile errors
        glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link orange shaders
        unsigned int shaderProgramOrange = glCreateProgram();
        glAttachShader(shaderProgramOrange, vertexShader);
        glAttachShader(shaderProgramOrange, fragmentShaderOrange);
        glLinkProgram(shaderProgramOrange);

        // check for linking error
        glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // link yellow shaders
        unsigned int shaderProgramYellow = glCreateProgram();
        glAttachShader(shaderProgramYellow, vertexShader);
        glAttachShader(shaderProgramYellow, fragmentShaderYellow);
        glLinkProgram(shaderProgramYellow);

        // check for linking error
        glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete shader after link to program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShaderOrange);
        glDeleteShader(fragmentShaderYellow);

        // Set up vertex and indices data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float firstTriangle[] = {
            -0.25f,  0.5f, 0.0f,  // top
            -0.5f, -0.5f, 0.0f,  // bottom left
            0.0f, -0.5f, 0.0f,  // bottom right
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
        // registered VBO as the vertex attribute's bound vertex buffer object
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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
            glUseProgram(shaderProgramOrange);
            // only have a single VAO there's no need to bind it every time, but do so to keep things a bit more organized
            glBindVertexArray(VAOs[0]);
            // draw rectangle
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // draw our second triangle
            glUseProgram(shaderProgramYellow);
            // only have a single VAO there's no need to bind it every time, but do so to keep things a bit more organized
            glBindVertexArray(VAOs[1]);
            // draw rectangle
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwPollEvents();
            glfwSwapBuffers(window);
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(2, VAOs);
        glDeleteBuffers(2, VBOs);
        glDeleteProgram(shaderProgramOrange);
        glDeleteProgram(shaderProgramYellow);

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
