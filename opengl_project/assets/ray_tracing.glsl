#version 430

layout(rgba16f, binding = 0) uniform highp writeonly image2D outTexture;
layout(local_size_x = 16, local_size_y = 16) in;
uniform float time;
uniform float random_array[110];

struct sphere
{
    vec3 center;
    float r;

    vec2 hit_sphere(vec3 o, vec3 d)
    {
        float a = dot(center, center);
        float b = 2 * dot(o - center, d);
        float c = dot(o - center, o - center) - r * r;
        float dis = b * b / 4 - c;

        if(dis < 0.0) return vec2(-1.0);
        dis = sqrt(dis);
        return vec2(-b / 2 - dis, -b / 2 + dis);
    }

    vec3 reflection(vec3 o, vec3 d)
    {
        return normalize(d - 2 * dot(normalize(o - center), d) * normalize(o - center));
    }
};

vec3 sky_color(vec3 r)
{
    float t = 0.5 * (r.y + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

float ground(vec3 o, vec3 d, vec4 floor)
{
    return -(dot(o, floor.xyz) + floor.w) / dot(d, floor.xyz);
}

vec3 ground_color(vec3 o, vec3 d, vec4 f, vec3 color)
{
    float t = ground(o, d, f);
    vec3 p = o + t * d;

    if(abs(dot(f.xyz, p) + f.w) == 0 && t >= 0) {return vec3(0.45);}
    return vec3(color);
}

void main(void)
{
    ivec3 xyIn = ivec3(gl_GlobalInvocationID.xyz);
    ivec2 imgsize = imageSize(outTexture).xy;
    if(xyIn.x >= imgsize.x) return;
    if(xyIn.y >= imgsize.y) return;

    float ratio = float(imgsize.x) / float(imgsize.y);
    vec3 origin = vec3(float(xyIn.x) / float(imgsize.x), float(xyIn.y) / float(imgsize.y), 0);
    origin.xy = ((2 * abs(origin.xy) - 1));
    origin.y /= -ratio;
    origin.x *= -1;

    sphere sphere1, sun;

    sphere1.center = vec3(0, 0, 25);
    sun.center = vec3(30 * sin(time / 10), 30, 30 * cos(time / 10));

    sphere1.r = 5;
    sun.r = 15;

    vec4 floor = vec4(0, 1, 0, 5);

    vec3 cam = vec3(0.0, 0.0, 0.5);
    vec3 direction;

    vec3 avg = vec3(0.0);

    vec3 real_point = vec3(0.0, 0.0, 0.5);
    float sizeLense = 0.01;
    float focus_distance = 5;
    vec3 focus_point = vec3(0.0);

    vec3 color = vec3(1.0);

    for(int i = -1; i < 50 ; i += 2)
    {
        real_point.xy = i == -1 ? vec2(0.0) : vec2(random_array[i], random_array[i + 1]) * sizeLense;
        real_point.z = 0.5;

        int check = 0;

        direction = normalize(real_point - origin);
        color = sky_color(direction);

        for(int j = 0; j < 8 && check == 0 ; ++j)
        {

            color = ground_color(origin, direction, floor, color);

            vec2 p = sphere1.hit_sphere(origin, direction);
            if(p.x >= 0 || p.y >= 0)
            {
                color = vec3(0.7);
                vec3 sp1 = origin + p.x * direction;
                vec3 sp2 = origin + p.y * direction;
                vec3 sphere_point = length(sp1) < length(sp2) ? sp1 : sp2;
                direction = sphere1.reflection(sphere_point, direction);
            }

            p = sun.hit_sphere(origin, direction);
            if(p.x >= 0 || p.y >= 0)
            {
                color = vec3(1, 249.0 / 255.0, 23.0 / 255.0);
                check = 1;
            }

            if(color == vec3(0.45))
            {
                float t = ground(origin, direction, floor);

                vec3 point = origin + t * direction;
                direction = normalize(sun.center - point);

                p = sphere1.hit_sphere(point, direction);

                if((p.x >= 0 || p.y >= 0) && length(point - sun.center) >= length(point - sphere1.center))
                {
                    color *= vec3(0.2);
                }

                direction = normalize(real_point - origin);
                direction = normalize(direction - 2 * dot(direction, floor.xyz) * floor.xyz);

                p = sphere1.hit_sphere(point, direction);

//                origin = point;

                if(p.x >= 0 || p.y >= 0)
                {
                    color *= vec3(0.7);
                }
                else
                {
                    color *= sky_color(direction);
                }
            }
        }

        avg += color;
    }

    avg /= 26.0;

    imageStore(outTexture, xyIn.xy, vec4(avg, 1.0));
}
