#version 330 core
out vec4 FragColor;

uniform sampler2D texture1; // Первая текстура
uniform sampler2D texture2; // Вторя текстура
in vec3 coords; // Координаты из вершинного шейдера
in vec3 normal; // Нормаль
in float type; // В зависимости от типа, выбирается текстура
in vec3 light;

void main()
{
    if(type == 0) {FragColor = texture(texture1, coords.xy) * dot(normal, normalize(light));}
    else {FragColor = texture(texture2, coords.xy) * dot(normal, normalize(light));}
}