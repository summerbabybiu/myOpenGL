#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>


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
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.1, 0.5, 0.0, 1.0);\n"
        "}\n";

//    unsigned int shader = CreateShader(vertexShader, fragmentShader);
//    glUseProgram(shader);

    //../res/shaders/Basic.shader
    ShaderProgramSource source = ParseShader("/Users/summerbaby/Desktop/summmbaby/myOpenGL/res/shaders/Basic.shader");
        // std::cout << source << std::endl;

    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

     unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
     glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {  
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // 下面这5行是自己加的，用来画一个三角形
        // glBegin(GL_TRIANGLES);
        // glVertex2d(-0.5, -0.5);
        // glVertex2d(0.5, -0.5);
        // glVertex2d(0, 0.5);
        // glEnd();

        glDrawArrays(GL_TRIANGLES, 0, 3); // 这一行是用来画一个三角形的

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}
