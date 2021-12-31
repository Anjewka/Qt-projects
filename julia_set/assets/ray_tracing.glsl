#version 430

#define pi 3.1415926535

layout(rgba8, binding = 1) uniform highp readonly image2D palette_texture;
layout(rgba16f, binding = 0) uniform highp writeonly image2D outTexture;
layout(local_size_x = 16, local_size_y = 16) in;
uniform float size;
uniform float width;
uniform float height;

void main(void)
{
    ivec3 xyIn = ivec3(gl_GlobalInvocationID.xyz);
    ivec2 imgsize = imageSize(outTexture).xy;
    if(xyIn.x >= imgsize.x) return;
    if(xyIn.y >= imgsize.y) return;

    float theta = 45.0;

    vec3 fake_point = vec3(0.0, 0.0, -0.5);
    float ratio = float(imgsize.x) / float(imgsize.y);
    vec3 p1 = vec3(float(xyIn.x) / float(imgsize.x), float(xyIn.y) / float(imgsize.y), 0.0);

    p1.xy = ((2 * abs(p1.xy) - 1));
    p1.y /= ratio;
    vec3 p2 = (p1 - fake_point);

//    p2 = normalize(p2);

    vec3 sphere1 = vec3(0, 0, 10);
    vec3 sun1 = vec3(0, 30, -10);

    float sphere_r = 2;
    float sun_r = 15;

    vec3 col = vec3(0, 0, 0);
//    float brightness = 0.8;

//    for(int i = 1; i<1000 ; ++i) {
//        p1 += p2;
//        if(length(p1 - sphere1) <= sphere_r) {
//            col = vec3(0.4);
//            p2 = p1 - sphere1;
//            p2 = p2;
//        }

//        if(length(p1 - sun1) <= sun_r) {
//            col = vec3(1, 1, 1);
//            break;
//        }
//    }

    imageStore(outTexture, xyIn.xy, vec4(col, 1.0));
}
