#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1050;

const char* vertexShaderSource =
"#version 460 core\n"

"layout (location = 0) in vec3 aPos;\n"

"out vec3 vertexColor;\n"

"uniform float timeValue;\n"

"void main()\n"
"{\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    
    "float spacing = 2.094;\n"

    "float vertexOffset = -float(gl_VertexID) * spacing;\n"

    "float redValue = cos(timeValue * 1.25 + vertexOffset + 0.0) / 2.0 + 0.5;\n"
    "float greenValue = cos(timeValue * 1.25 + vertexOffset + spacing) / 2.0 + 0.5;\n"
    "float blueValue = cos(timeValue * 1.25 + vertexOffset + spacing * 2) / 2.0 + 0.5;\n"

    "vertexColor = vec3(redValue, greenValue, blueValue);\n"
"}\n";

const char* fragmentShaderSource =
    "#version 460 core\n"

    "out vec4 FragColor;\n"
    "in vec3 vertexColor;\n"

    "void main()\n"
    "{\n"
        "FragColor = vec4(vertexColor, 1.0);\n"
    "}\n";

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);

    if (window == NULL)
    {
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
	    std::cout << "Failed to initialize GLAD" << std::endl;
    	return -1;
    }

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = 
    {
        // positions
         0.65f, -0.65f, 1.0f,  // bottom right
        -0.65f, -0.65f, 1.0f,  // bottom left
         0.0f,  0.65f, 1.0f   // top
    };

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
    	processInput(window);

    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        int timeValueLocation = glGetUniformLocation(shaderProgram, "timeValue");
        glUniform1f(timeValueLocation, timeValue);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

	    glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate objects/programs
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}