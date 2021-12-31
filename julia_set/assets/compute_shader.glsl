#version 430

layout(rgba8, binding = 1) uniform highp readonly image2D palette_texture;
layout(rgba16f, binding = 0) uniform highp writeonly image2D outTexture;
layout(local_size_x = 16, local_size_y = 16) in;
uniform float size;
uniform float width;
uniform float height;
uniform float x0;
uniform float y0;

#define type float

float reinhard(float v)
{
    return 64 * v / (50 + v);
}

void main() {
    ivec2 xyIn = ivec2(gl_GlobalInvocationID.xy);
    ivec2 imgsize = imageSize(outTexture).xy;
    ivec2 psize = imageSize(palette_texture).xy;
    if(xyIn.x >= imgsize.x) return;
    if(xyIn.y >= imgsize.y) return;

    type x = type(xyIn.x), y = type(xyIn.y);
    type zx = x - width / 2, zy = y - height / 2;
    zx = zx / size + x0; zy = zy / size + y0;
    type x1 = zx; type y1 = zy;

    int i = 0;
    while(sqrt(zx * zx + zy * zy) < 4.0 && i < 1000)
    {
        type tmp = zx * zx - zy * zy;
        zy = zx * zy + zx * zy;
        zx = tmp;
        zx += -0.8; zy += 0.156;
        ++i;
    }

    float mu = float(i) + 1 - log2(log2(sqrt(zx * zx + zy * zy)) / log(2.0));

    mu = reinhard(mu);

    int color1 = int(mu);
    int color2 = (color1 + 1) % psize.x;
    float t2 = mu - color1;
    float t1 = 1 - t2;
    color1 %= psize.x;

    vec4 colors1 = (imageLoad(palette_texture, ivec2(color1, 0)));
    vec4 colors2 = (imageLoad(palette_texture, ivec2(color2, 0)));

    float r = (colors1.x * t1 + colors2.x * t2);
    float g = (colors1.y * t1 + colors2.y * t2);
    float b = (colors1.z * t1 + colors2.z * t2);

//    vec3 vec = vec3(zx, zy, 1);
//    vec = normalize(vec);

//    vec2 v = vec2(0.7, 0.7);
//    float t = vec.x * v.x + vec.y * v.y + 1.5;
//    t = t / (2.5);

//    t = mix(1, 255, t);

    imageStore(outTexture, xyIn, vec4(r, g, b, 1.0));
}
