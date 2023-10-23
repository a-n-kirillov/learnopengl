#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

void cleanupAndExit()
{
    glfwTerminate();
    exit(EXIT_FAILURE);
}

void verifyWindowCreation(GLFWwindow* window)
{
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        cleanupAndExit();
    }
}

void initializeGladFunctionLoader()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        cleanupAndExit();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initializeGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

unsigned int setupVAO(float *vertices, int verticesSize)
{
    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return VAO;
}

void checkShaderCompilationErrors(unsigned int shaderObject)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int createCompiledVertexShaderObject(const char *vertexShaderSource)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompilationErrors(vertexShader);

    return vertexShader;
}

unsigned int createCompiledFragmentVertexObject(const char *fragmentShaderSource)
{
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompilationErrors(fragmentShader);

    return fragmentShader;
}

void checkLinkingErrors(unsigned int shaderProgram)
{
    int  success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int createShadingProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkLinkingErrors(shaderProgram);

    return shaderProgram;
}

int main()
{
    initializeGLFW();
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    verifyWindowCreation(window);
    glfwMakeContextCurrent(window);
    initializeGladFunctionLoader();
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    unsigned int vertexShader = createCompiledVertexShaderObject(vertexShaderSource);

    const char *fragmentShaderSourceOrange = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "\n"
                                       "void main()\n"
                                       "{\n"
                                       "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "} ";
    const char *fragmentShaderSourceYellow = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "\n"
                                       "void main()\n"
                                       "{\n"
                                       "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                       "} ";
    unsigned int fragmentShaderOrange = createCompiledFragmentVertexObject(fragmentShaderSourceOrange);
    unsigned int fragmentShaderYellow = createCompiledFragmentVertexObject(fragmentShaderSourceYellow);

    unsigned int orangeShadingProgram = createShadingProgram(vertexShader, fragmentShaderOrange);
    unsigned int yellowShadingProgram = createShadingProgram(vertexShader, fragmentShaderYellow);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOrange);
    glDeleteShader(fragmentShaderYellow);

    float firstTriangleVertices[] = {
            -1.0f, -1.0f, 0.0f,
            -0.5f, -1.0f, 0.0f,
            -0.75f, 1.0f, 0.0f

    };
    float secondTriangleVertices[] = {
            0.5f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.75f, 1.0f, 0.0f
    };

    unsigned int firstTriangleVAO = setupVAO(firstTriangleVertices, sizeof(firstTriangleVertices));
    unsigned int secondTriangleVAO = setupVAO(secondTriangleVertices, sizeof(secondTriangleVertices));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(orangeShadingProgram);
        glBindVertexArray(firstTriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(yellowShadingProgram);
        glBindVertexArray(secondTriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        processInput(window);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
