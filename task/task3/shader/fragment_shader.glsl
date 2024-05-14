#version 330 core
out vec4 FragColor;

uniform sampler2D texture1; // Первая текстура
in vec3 coords; // Координаты из вершинного шейдера
in vec3 normal; // Нормаль
in float type; // В зависимости от типа, выбирается текстура
in vec3 light;

void main()
{
    FragColor = texture(texture1, coords.xy) * dot(normal, normalize(light));
}