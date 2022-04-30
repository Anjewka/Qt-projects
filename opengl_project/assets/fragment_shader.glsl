#version 330

uniform sampler2D InputTex;


void main() {
    ivec2 xy = ivec2(gl_FragCoord.xy);
    gl_FragColor = texelFetch(InputTex, xy, 0);
}
