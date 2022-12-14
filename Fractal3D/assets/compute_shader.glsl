#version 430

layout(rgba8, binding = 1) uniform highp readonly image2D palette_texture;
layout(rgba16f, binding = 0) uniform highp writeonly image2D outTexture;
layout(local_size_x = 16, local_size_y = 16) in;
uniform float size;
uniform float width;
uniform float height;
uniform float angel_x;
uniform float angel_y;
uniform float z0;
uniform float arr[4];

int numIterations = 300;

#define type float
#define PI 3.1415926538

#define TRAP

int POWER = 8;
vec4 Constant = vec4(-0.4, 0.6, 0, 0);

vec4 Mul(vec4 q1, vec4 q2)
{
    return vec4(q1.x * q2.x - dot(q1.yzw, q2.yzw), q2.x * q1.yzw + q1.x * q2.yzw + cross(q1.yzw, q2.yzw));
}

float qlength2( in vec4 q )
{
    return dot(q,q);
}

vec4 qsqr( in vec4 a ) // square a quaterion
{
    return vec4( a.x*a.x - a.y*a.y - a.z*a.z - a.w*a.w,
                 2.0*a.x*a.y,
                 2.0*a.x*a.z,
                 2.0*a.x*a.w );
}

vec4 qconj(in vec4 a)
{
    return vec4(a.x, -a.yzw);
}

vec4 Square(vec4 q)
{
    return vec4(q.x * q.x - q.y * q.y - q.z * q.z - q.w * q.w, 2.0 * q.x * q.yzw);
}

float Length(vec4 q) {return dot(q, q);}

vec4 Pow(vec4 z, int n) {
    vec4 z0 = vec4(1.0, vec3(0.0));
    for(int i = 0; i < n; i++)
            z0 = Mul(z0, z);
    return z0;
}

float reinhard(float v)
{
    return 64 * v / (50 + v);
}

vec3 Mandelbuld_distance(vec3 p) {
    vec4 z = vec4(p, 0);
    float dr = 1.0;
    float r = 0.0;
    int i = 0;
    while(i < numIterations)
    {
        r = length(z);
        if (r > 2.51) {break;}

        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        theta = theta * POWER;
        phi = phi * POWER;

        z = zr * vec4(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta), 0);
        z += vec4(p, 0);
        ++i;
    }

    float d = 0.25 * log(r) * r / dr;

    return vec3(d, i, length(z));
}

vec3 Julia_set_distance(vec3 p)
{
    vec4 z = vec4(p, 0.0);
    float dz2 = 1.0;
    float m2 = 0.0;
    float n = 0.0;

    float o = 199;

    for(int i = 0; i < numIterations; i++)
    {
        dz2 *= 4 * length(z);
        z = Pow(z, 2) + Constant;
        m2 = length(z);
        n += 1.0;

        if(m2 > 8.0) break;
    }

    float d = 200;
    if(dz2 != 0 && m2 / dz2 >= 0) {d = 0.25 * log(m2) * sqrt(m2 / dz2);}


    #ifdef TRAPS
    d = min(o, d);
    #endif
    #ifdef CUT
    d = max(d, min(o, p.z));
    #endif

    return vec3(d, n, length(m2));
}

vec3 rotate(vec3 point)
{
    point = point * mat3(cos(angel_x), 0, sin(angel_x),
                        0, 1, 0,
                        -sin(angel_x), 0, cos(angel_x));
    point = point * mat3(1, 0, 0,
                         0, cos(angel_y), -sin(angel_y),
                         0, sin(angel_y), cos(angel_y));
    return point;
}

vec3 calcNormal(vec3 p)
{
    vec4 z = vec4(p,0.0);

    // identity derivative
    vec4 J0 = vec4(1,0,0,0);
    vec4 J1 = vec4(0,1,0,0);
    vec4 J2 = vec4(0,0,1,0);

    for(int i=0; i<numIterations; i++)
    {
        vec4 cz = qconj(z);

        // chain rule of jacobians (removed the 2 factor)
        J0 = vec4( dot(J0,cz), dot(J0.xy,z.yx), dot(J0.xz,z.zx), dot(J0.xw,z.wx) );
        J1 = vec4( dot(J1,cz), dot(J1.xy,z.yx), dot(J1.xz,z.zx), dot(J1.xw,z.wx) );
        J2 = vec4( dot(J2,cz), dot(J2.xy,z.yx), dot(J2.xz,z.zx), dot(J2.xw,z.wx) );

        // z -> z2 + c
        z = qsqr(z) + Constant;

        if(qlength2(z) > 8.0) break;
    }

    vec3 v = vec3( dot(J0,z),
                   dot(J1,z),
                   dot(J2,z) );

    return normalize(v);
}

void main() {
    ivec2 xyIn = ivec2(gl_GlobalInvocationID.xy);
    ivec2 imgsize = imageSize(outTexture).xy;
    ivec2 psize = imageSize(palette_texture).xy;
    if(xyIn.x >= imgsize.x) return;
    if(xyIn.y >= imgsize.y) return;

//    Constant.x = arr[0];
//    Constant.y = arr[1];

    float ratio = float(imgsize.x) / float(imgsize.y);
    vec3 origin = vec3(float(xyIn.x) / float(imgsize.x), float(xyIn.y) / float(imgsize.y), 0);
    origin.xy = ((2 * abs(origin.xy) - 1));
    origin.y /= -ratio;
    origin.x *= -1;
    origin.z = z0;

    vec3 real_point = vec3(0, 0, z0 + 0.3);
    vec3 light = (vec3(-1, -1, -1));
    light = rotate(light);
    origin = rotate(origin);
    vec3 camera = origin;
    real_point = rotate(real_point);

    vec3 direction;

    vec3 color = vec3(0);

    direction = normalize(real_point - origin);

    int i = 0;
    bool c = false;
    int max_iteration = 300;

    while(i < max_iteration)
    {
        vec3 dist = Julia_set_distance(origin);

        if(dist.x <= 0.00035)
        {
            float mu = float(dist.y) + 1 - log2(log2((dist.z)) / log(2.0));

            mu = reinhard(mu);

            int color1 = int(mu);
            int color2 = ((color1 + 1) % 16);
            float t2 = mu - color1;
            float t1 = 1 - t2;
            color1 %= 16;

            vec4 colors1 = (imageLoad(palette_texture, ivec2(color1, 0)));
            vec4 colors2 = (imageLoad(palette_texture, ivec2(color2, 0)));

            float r = (colors1.x * t1 + colors2.x * t2);
            float g = (colors1.y * t1 + colors2.y * t2);
            float b = (colors1.z * t1 + colors2.z * t2);
            vec2 uv = origin.xy / xyIn.xy;

            color = vec3(r, g, b);
            vec3 norm = calcNormal(origin);
            color = color * dot(norm, light);
            break;
        }
        origin = origin + dist.x * direction;
        ++i;
    }

    imageStore(outTexture, xyIn, vec4(color, 1));
}
