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

#define type float
#define PI 3.1415926538

#define TRAP

int POWER = 2;
vec4 Constant = vec4(-0.8, 0.156, 0, 0);

vec4 Mul(vec4 q1, vec4 q2)
{
    return vec4(q1.x * q2.x - dot(q1.yzw, q2.yzw), q2.x * q1.yzw + q1.x * q2.yzw + cross(q1.yzw, q2.yzw));
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

float get_fractal_dist(vec4 p)
{
    int i = 0;
    vec4 c = p;
    vec4 z = vec4(0);

    while(length(z.xyz) < 2.50 && i < 100)
    {
        z = Pow(z, 3);
        z += c;
        ++i;
    }

    float mu = float(i) + 1 - log2(log2(sqrt(float(z.x * z.x + z.y * z.y + z.z * z.z))) / log(2.0));
    return mu;
}

vec3 colorSurface(vec3 pos, vec3 nor, vec2 tn )
{
    vec3 col = 0.5+0.5*cos(log2(tn.y)*0.9+3.5+vec3(0.0,0.6,1.0));
    if( pos.y>0.0 ) col = mix(col,vec3(1.0),0.2);
    float inside = smoothstep(14.0,15.0,tn.y);
    col *= vec3(0.45,0.42,0.40) + vec3(0.55,0.58,0.60)*inside;
    col = mix(col*col*(3.0-2.0*col),col,inside);
    col = mix( mix(col,vec3(dot(col,vec3(0.3333))),-0.4),
                        col, inside);
    return clamp(col*0.65,0.0,1.0);
}

vec3 Mandelbuld_distance(vec3 p) {
    vec4 z = vec4(p, 0);
    float dr = 1.0;
    float r = 0.0;
    int i = 0;
    while(i < 200)
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
    float tmp = 0.5 * log(r) * r / dr;
    return vec3(tmp, i, z.x * z.x + z.y * z.y + z.z * z.z);
}

vec3 Julia_set_distance(vec3 p)
{
    vec4 z = vec4(p, 0.0);
    float dz2 = 1.0;
    float m2 = 0.0;
    float n = 0.0;

    float o = 1e10;

    for(int i = 0; i < 200; i++)
    {
        dz2 *= 9 * Length(Pow(z, 3));
        z = Pow(z, 3) + Constant;
        m2 = Length(z);

        if(m2 > 17) break;
        n += 1.0;
    }

    float d = 0.25 * log(m2) * sqrt(m2 / dz2);

    #ifdef TRAPS
    d = min(o, d);
    #endif
    #ifdef CUT
    d = max(d, p.y);
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

void main() {
    ivec2 xyIn = ivec2(gl_GlobalInvocationID.xy);
    ivec2 imgsize = imageSize(outTexture).xy;
    ivec2 psize = imageSize(palette_texture).xy;
    if(xyIn.x >= imgsize.x) return;
    if(xyIn.y >= imgsize.y) return;

    float ratio = float(imgsize.x) / float(imgsize.y);
    vec3 origin = vec3(float(xyIn.x) / float(imgsize.x), float(xyIn.y) / float(imgsize.y), 0);
    origin.xy = ((2 * abs(origin.xy) - 1));
    origin.y /= -ratio;
    origin.x *= -1;
    origin.z = z0;

    vec3 real_point = vec3(0, 0, z0 + 0.3);
    origin = rotate(origin);
    real_point = rotate(real_point);

    vec3 direction;

    vec3 color = vec3(1);

    direction = normalize(real_point - origin);

    int i = 0;
    bool c = false;
    int max_iteration = 300;

    while(i < max_iteration)
    {
        vec3 dist = Julia_set_distance(origin);

        if(dist.x < 0.01)
        {
            float mu = float(dist.y) + 1 - log2(log2(sqrt(dist.z)) / log(2.0));

            mu = reinhard(mu);

            int color1 = int(mu);
            int color2 = (color1 + 1) % 8 + 8;
            float t2 = mu - color1;
            float t1 = 1 - t2;
            color1 %= 8;
            color1 += 8;

            vec4 colors1 = (imageLoad(palette_texture, ivec2(color1, 0)));
            vec4 colors2 = (imageLoad(palette_texture, ivec2(color2, 0)));

            float r = (colors1.x * t1 + colors2.x * t2);
            float g = (colors1.y * t1 + colors2.y * t2);
            float b = (colors1.z * t1 + colors2.z * t2);
            color = vec3(r, g, b);
            break;
        }
        origin = origin + dist.x * direction;
        ++i;
    }

    imageStore(outTexture, xyIn, vec4(color, 1));
}
