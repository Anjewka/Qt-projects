#version 330 core
layout (location = 0) in vec3 position; // Координаты вершины
layout (location = 1) in vec3 norm; // Нормаль
layout (location = 2) in vec3 texture_position; // Координаты текстуры

uniform mat4 camera; // Камера
uniform mat4 rotation; // матрица поворота камеры
uniform float time; // время

out vec3 coords; // Координаты текстуры
out vec3 normal; // Нормаль
out float type; // Тип
out vec3 light; // Вектор света

void main()
{
    gl_Position = camera * rotation * vec4(position, 1.0);
    normal = norm;
    coords = texture_position;
    type = texture_position.z;
    light = vec3(1, 1, 0);
}