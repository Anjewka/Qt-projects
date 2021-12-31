#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 coord;

out vec2 coords;

void main() {
    coords=coord.st;
    gl_Position = vec4(pos, 1.0);
}
