#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 norm;

uniform mat4 camera;
uniform mat4 rotation;
uniform float time;

out vec3 color_vertex;

void main()
{
    gl_Position = camera * rotation * vec4(aPos, 1.0);

    // Эллипс - проекция окружности на плоскость
    // Чтобы отобразить эллипс, отобразим окружность в плоскости Oxz,
    // на данную в задание(-x -y + 2z + 20 = 0), при этом x, z остануться неизменным
    float x = 15 * cos(time);
    float z = 15 * sin(time);
    float y = -x + 2 * z + 20;

    vec3 light = -normalize(vec3(x, y, z));

    color_vertex = color * dot(norm, light);
}