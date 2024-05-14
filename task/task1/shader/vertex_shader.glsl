#version 330 core
layout (location = 0) in vec3 position; // Позиция камеры
layout (location = 1) in vec3 color; // Цвет вершины
layout (location = 1) in vec3 normal; // Нормаль
  
uniform mat4 camera; // Камера
uniform mat4 rotation; // Матрица поворота камеры
out vec3 vertexColor; // Цвет

void main()
{
    gl_Position = camera * rotation * vec4(position, 1.0);
    vertexColor = color * dot(normal, normalize(vec3(1.0, 1.0, 1.0)));
}