#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::cout << "filepath: " << filepath << std::endl;
    std::ifstream stream;
    stream.open(filepath);
    // std::ifstream stream(filepath);

    if (!stream) {
        std::cout << "open file failed" << std::endl;
    } else {
        std::cout << "open file success" << std::endl;
    }
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    

    while (getline(stream, line)) {
         std::cout << "hahaha" << std::endl;
        std::cout << line << std::endl;
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        } else {
                std::cout << line << std::endl;

            ss[(int)type] << line << '\n';   
        }
    }
    std::cout << "ss[0].str()" << std::endl;
    std::cout << ss[0].str() << std::endl;
    std::cout << "ss[1].str()" << std::endl;
    std::cout << ss[1].str() << std::endl;
    return {
        ss[0].str(),
        ss[1].str()
    };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    // const char* src = &source[0]; 
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs); // vs: vertex shader
    glAttachShader(program, fs); // fs: fragment shader
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "summerbaby Hello", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK) {
        std::cout << "glew init failed" << std::endl;
    } else {
        std::cout << "glew init success" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;


    // glewInit(); // 这一行要放在创建窗口之后，不然会报错

    float positions[] = {
        -0.5f, -0.5f, // 0
        0.5f,  -0.5f, // 1
        0.5f, 0.5f, // 2
        -0.5f, 0.5f, // 3
    };
    // 索引缓冲区index buffer， 必须由unsigned int类型
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0 
    };
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    // 启用顶点属性
    glEnableVertexAttribArray(0);
    // 指定实际数据的布局
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    IndexBuffer ib(indices, 6);

    //../res/shaders/Basic.shader
    ShaderProgramSource source = ParseShader("/Users/summerbaby/Desktop/summmbaby/myOpenGL/res/shaders/Basic.shader");
        // std::cout << source << std::endl;

    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    // 解绑
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r =0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {  
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // 绑定着色器
        GLCall(glUseProgram(shader));
        // 设置统一变量
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        // // 绑定顶点缓冲区
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        // // 启用顶点属性
        // GLCall(glEnableVertexAttribArray(0));
        // 指定实际数据的布局
        // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        // 绑定索引缓冲区
        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBindVertexArray(vao));
        ib.Bind();

        GLCall( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)) ; // 这一行是用来画一个正方形的
        // GLCheckError();
        // ASSERT(GLLogCall());
        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment = 0.05f;
        }

        r += increment;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glfwSwapInterval(1);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}
