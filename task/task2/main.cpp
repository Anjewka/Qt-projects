#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Для удобства
void push_back(std::vector<float>& vec, const std::vector<glm::vec3>& vec2, glm::vec3 norm)
{
    for(size_t i = 0; i<vec2.size(); ++i)
    {
        vec.push_back(vec2[i].x);
        vec.push_back(vec2[i].y);
        vec.push_back(vec2[i].z);
    }
    vec.push_back(norm.x);
    vec.push_back(norm.y);
    vec.push_back(norm.z);
}

// Указатель для использования в static функциях
static void* class_pointer;
struct OpenglWindow
{
    // Соотношение сторон
    float aspect;
    // Координаты мышки
    double mouse_x, mouse_y;
    // Углы поворота камеры
    double angel_x, angel_y;

    GLFWwindow *window;
    // Матрица камеры
    glm::mat4 camera;
    glm::vec3 current_pos;
    // Матрица вращения
    glm::mat4 rotation;

    // Конструктор
    OpenglWindow(float aspect_width, float aspect_height)
    {
        if (!glfwInit()) {exit(1);} //Init glfw

        const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        aspect = vidmode->width * aspect_width / (vidmode->height * aspect_height);        
        camera = glm::perspective(55.0f, aspect, 0.01f, 100.0f);
        current_pos = glm::vec3(-8, 0, 8);
        camera *= glm::lookAt(current_pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        rotation = glm::mat4(1.0);     

        createWindow("Window", (int)(vidmode->width * aspect_height), (int)(vidmode->height * aspect_height));

        if (glewInit() != GLEW_OK) {
            exit(1);
        }
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        angel_x = 0, angel_y = 0;
        
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    // Сигнал движения мышки
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position)
    {
        auto app = (OpenglWindow*)class_pointer;
        float delta_x = x_position - app->mouse_x;
        float delta_y = app->mouse_y - y_position;
        app->mouse_x = x_position;
        app->mouse_y = y_position;

        const float sensitivity = 0.001f;
        delta_x *= sensitivity;
        delta_y *= sensitivity;

        if(glfwGetMouseButton(app->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            app->rotation = glm::mat4(1.0);

            app->angel_x -= delta_x;
            app->angel_y += delta_y;

            app->rotation = glm::rotate(app->rotation, float(app->angel_y), glm::vec3(1, 0, 0));
            app->rotation = glm::rotate(app->rotation, float(app->angel_x), glm::vec3(0, 0, -1));
        }
    }

    // Создание окна
    void createWindow(const string &name, const int &width, const int &height) {
        window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);
        if (window == nullptr) {
            std::cout << "Failed to create GLFW window" << '\n';
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(window);
    }

    void render()
    {
        float time = 0;
        // Вершины из задания
        std::vector<glm::vec3> A = {glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)};
        std::vector<glm::vec3> B = {glm::vec3(0, 3, 0), glm::vec3(0, 21.0 / 255.0, 21.0 / 255.0)};
        std::vector<glm::vec3> C = {glm::vec3(3, 3, 0), glm::vec3(191.0 / 255.0, 0, 0)};
        std::vector<glm::vec3> D = {glm::vec3(3, -3, 0), glm::vec3(1, 1, 0)};
        std::vector<glm::vec3> E = {glm::vec3(0, -3, 0), glm::vec3(128.0 / 255.0, 0, 1)};
        std::vector<glm::vec3> M = {glm::vec3(0, 0, 5), glm::vec3(1, 1, 1)};

        // Нормали граней(векторное вросизведение сторон)
        glm::vec3 norm1 = glm::normalize(glm::cross(A[0] - B[0], A[0] - C[0]));
        glm::vec3 norm2 = glm::normalize(glm::cross(D[0] - B[0], D[0] - M[0]));
        glm::vec3 norm3 = glm::normalize(glm::cross(B[0] - C[0], B[0] - M[0]));
        glm::vec3 norm4 = glm::normalize(glm::cross(C[0] - D[0], C[0] - M[0]));
        glm::vec3 norm5 = glm::normalize(glm::cross(D[0] - E[0], D[0] - M[0]));


        // Вектор вершин
        vector<float> vertices;

        // В строчку первые 3 значения - координаты вершины, следущие 3 - цвет
        // последние 3 - нормаль
        // Основание -----------------------------------
        push_back(vertices, A, norm1);
        push_back(vertices, B, norm1);
        push_back(vertices, C, norm1);

        push_back(vertices, A, norm1);
        push_back(vertices, C, norm1);
        push_back(vertices, D, norm1);

        push_back(vertices, A, norm1);
        push_back(vertices, E, norm1);
        push_back(vertices, D, norm1);

        // Грани--------------------------------------
        push_back(vertices, E, norm2);
        push_back(vertices, B, norm2);
        push_back(vertices, M, norm2);

        push_back(vertices, B, norm3);
        push_back(vertices, C, norm3);
        push_back(vertices, M, norm3);

        push_back(vertices, C, norm4);
        push_back(vertices, D, norm4);
        push_back(vertices, M, norm4);

        push_back(vertices, D, norm5);
        push_back(vertices, E, norm5);
        push_back(vertices, M, norm5);

        int layoutsize = 9;

        GLuint currentProgram = createProgram(readGlsl("../shader/vertex_shader.glsl"), readGlsl("../shader/fragment_shader.glsl"));

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertices.size()), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Координаты вершины
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layoutsize * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Цвет вершины
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, layoutsize * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Нормаль
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, layoutsize * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBindVertexArray(VAO);
        glEnable(GL_DEPTH_TEST);

        // Цикл рендера
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            glUseProgram(currentProgram);
            // Задаем значение для камеры
            GLint camera_location = glGetUniformLocation(currentProgram, "camera");
            glUniformMatrix4fv(camera_location, 1, GL_FALSE, glm::value_ptr(camera));
            // Вращение камеры
            GLint rotate_location = glGetUniformLocation(currentProgram, "rotation");
            glUniformMatrix4fv(rotate_location, 1, GL_FALSE, glm::value_ptr(rotation));
            // время
            GLint time_location = glGetUniformLocation(currentProgram, "time");
            glUniform1f(time_location, time);
            // Отрисовка треугольников
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / layoutsize);
            glfwSwapBuffers(window);
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            time += 0.01;
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    // Создает вершинный и фрагментый шейдеры
    static GLuint createProgram(const string &vertexProgram, const string &fragmentProgram) {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        char *pVertexProgram = const_cast<char *>(vertexProgram.c_str());
        char *pFragmentProgram = const_cast<char *>(fragmentProgram.c_str());
        glShaderSource(vertexShader, 1, &pVertexProgram, nullptr);
        glShaderSource(fragmentShader, 1, &pFragmentProgram, nullptr);
        glCompileShader(vertexShader);
        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(vertexShader);
            cout << "VertexShader error:" <<(const char*)&infoLog[0] << '\n';
            return 0;
        }
        glCompileShader(fragmentShader);
        isCompiled = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(fragmentShader);
            cout << "FragmentShader error:" << (const char*)&infoLog[0] << '\n';
            return 0;
        }
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            cout << "Program error:" << (const char*)&infoLog[0] << '\n';
            return 0;
        }
        return program;
    }

    // Запись шейдера в строку
    static string readGlsl(const string &name) {
        std::ifstream file(name);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

int main() {
    OpenglWindow *window = new OpenglWindow(0.999, 0.9);
    class_pointer = window;
    window->render();
    return 0;
}