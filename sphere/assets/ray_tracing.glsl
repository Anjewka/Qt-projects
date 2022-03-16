#version 430

#define pi 3.1415926535

layout(rgba8, binding = 1) uniform highp readonly image2D palette_texture;
layout(rgba16f, binding = 0) uniform highp writeonly image2D outTexture;
layout(local_size_x = 16, local_size_y = 16) in;
uniform float size;
uniform float x0;
uniform float y0;
//uniform float z0;

#define sky vec3(117.0 / 255.0, 187.0 / 255.0, 253.0 / 255.0)
#define ground vec3(0.45)
#define sun vec3(1, 249.0 / 255/0, 23.0 / 255.0)

float floor_vec(vec3 p1, vec3 p2, vec4 f)
{
//    if((f.x * p2.x + f.y * p2.y + f.z * p2.z) != 0)
//    {
        return -(f.x * p1.x + f.y * p1.y + f.z * p1.z + f[3]) / (f.x * p2.x + f.y * p2.y + f.z * p2.z);
//    }
}

vec4 sphere_vec(vec3 p1, vec3 p2, vec3 s, float r)
{
    float a = p2.x * p2.x + p2.y * p2.y + p2.z * p2.z;
    float b = (2 * p1.x * p2.x - 2 * p2.x * s.x) + (2 * p1.y * p2.y - 2 * p2.y * s.y) + (2 * p1.z * p2.z - 2 * p2.z * s.z);
    float c = (p1.x * p1.x - 2 * s.x * p1.x + s.x * s.x) + (p1.y * p1.y - 2 * s.y * p1.y + s.y * s.y)
            + (p1.z * p1.z - 2 * s.z * p1.z + s.z * s.z) - r * r ;
    float d = b * b - 4 * a * c;
    return vec4(a, b, c, d);
}

vec3 get_point(vec3 p1, vec3 p2, float t1)
{
    return vec3(p1.x + p2.x * t1, p1.y + p2.y * t1, p1.z + p2.z * t1);
}

vec2 get_t(vec4 d)
{
    return vec2((-d.y + sqrt(d[3])) / (2 * d.x), (-d.y - sqrt(d[3])) / (2 * d.x));
}

void main(void)
{
    ivec3 xyIn = ivec3(gl_GlobalInvocationID.xyz);
    ivec2 imgsize = imageSize(outTexture).xy;
    if(xyIn.x >= imgsize.x) return;
    if(xyIn.y >= imgsize.y) return;

    vec3 fake_point = vec3(0, 0, -0.5);
    float ratio = float(imgsize.x) / float(imgsize.y);
    vec3 p1 = vec3(float(xyIn.x) / float(imgsize.x), float(xyIn.y) / float(imgsize.y), 0.0);

    p1.xy = ((2 * abs(p1.xy) - 1));
    p1.y /= ratio;
    vec3 p2 = (p1 - fake_point);

    p2 = normalize(p2);

    vec3 sphere1 = vec3(0, 0, 35);
    vec3 sun1 = vec3(25 * cos(size / 10), 20, 25 - 25 * sin(size / 10));

    float sphere_r = 5;
    float sun_r = 10;

    vec3 col = ground;

    vec4 floor = vec4(0.0, 1.0, 0.0, 5.0);
    float w = 200, h = 200;

    vec4 d = sphere_vec(p1, p2, sphere1, sphere_r);

    if(d[3] >= 0)
    {
        col = vec3(0.7);

        vec2 t = get_t(d);

        vec3 tmp1 = get_point(p1, p2, t.x);
        vec3 tmp2 = get_point(p1, p2, t.y);

        vec3 p3;
        vec3 p4;

        if(length(tmp1) <= length(tmp2)) {p3 = tmp1;}
        else {p3 = tmp2;}

        p4 = vec3(p3 - sphere1);
        p4 = normalize(p4);

        vec4 d1 = sphere_vec(p3, p4, sun1, sun_r);

        if(d1[3] >= 0)
        {
            t = get_t(d1);

            if(t.x >= 0 || t.y >= 0)
            {
                vec3 p6 = get_point(p3, p4, t.x);
                vec3 p7 = get_point(p3, p4, t.y);

                p6 = length(p6 - sun1) < length(p7 - sun1) ? p6 : p7;

                d = sphere_vec(sphere1, normalize(sphere1 - sun1), sphere1, sphere_r);
                t = get_t(d);
                tmp1 = get_point(sphere1, normalize(sphere1 - sun1), t.x);
                tmp2 = get_point(sphere1, normalize(sphere1 - sun1), t.y);

                float dist = min(length(tmp1 - sun1), length(tmp2 - sun1));
                float dist2 = length(tmp1 - p6);


                col = sun;
            }
            else
            {
                float time = length(sun1 - p3);
                col = vec3(0.3);
            }
        }
        else
        {
            p4 = normalize(sun1 - p3);
            d = sphere_vec(p3, p4, sphere1, sphere_r);

            if(d[3] >= 0)
            {
                t = get_t(d);

                tmp1 = get_point(p3, p4, t.x);
                tmp2 = get_point(p3, p4, t.y);

                if((length(sun1 - tmp1) <= length(sun1 - tmp2) && length(tmp2 - p3) <= length(vec3(0.15))) || (length(sun1 - tmp1) > length(sun1 - tmp2) && length(tmp1 - p3) <= length(vec3(0.15))))
                {
                    float time = length(tmp1 - tmp2);
                    col = vec3(max(0.7 - time / (sphere_r * 1.5), 0.3));
                }
            }
        }



//        else
//        {
//            p4 = normalize(sun1 - p3);
//            d = sphere_vec(p3, p4, sphere1, sphere_r);

//            t = get_t(d);
//            tmp1 = get_point(p3, p4, t.x);
//            tmp2 = get_point(p3, p4, t.y);

//            if(length(tmp1) <= length(tmp2) && length(sun1 - tmp1) >= length(sun1 - tmp2)) {col = vec3(0.2);}
//            else if(length(tmp1) > length(tmp2) && length(sun1 - tmp1) < length(sun1 - tmp2)) {col = vec3(0.2);}
//        }
    }

    if(col == ground)
    {
        float ft = floor_vec(p1, p2, floor);
        vec3 floor_point = get_point(p1, p2, ft);

        if(floor_point.x < h && floor.x > -h && floor_point.z < w && floor_point.y == -floor[3] && ft > 0)
        {
            col = ground;
            vec3 p3 = normalize(sun1 - floor_point);

            vec4 d1 = sphere_vec(floor_point, p3, sphere1, sphere_r);

            if(d1[3] >= 0)
            {
                vec2 t = get_t(d1);
                p1 = get_point(floor_point, p3, t.x);
                p2 = get_point(floor_point, p3, t.y);
                if(length(p1 - sphere1) <= length(p1 - sun1)) {col = vec3(0.2);}
            }
        }
        else {col = sky;}
    }

    imageStore(outTexture, xyIn.xy, vec4(col, 1));
}
