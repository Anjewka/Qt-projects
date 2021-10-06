void kernel main(global uchar4* out, global const float* input, global const uchar3* colors)
{
    const int ix = get_global_id(0);
    const int iy = get_global_id(1);

    const float w = input[6]; const float h = input[7]; const float size = input[4];
    const float x0 = input[2]; const float y0 = input[3]; const float mi = input[5];
    const float re = input[0]; const float im = input[1];

    float2 z = (float2)((float)(ix - w / 2), (float)(iy - h / 2));
    z = z / size;
    z = z + (float2)(x0, y0);

    int i = 0;
    for(int j = 0; j<mi ; ++j)
    {
        if(length(z) > 4.0) {i = j; break;}
        float tmp = z.x * z.x - z.y * z.y;
        z.y = z.x * z.y + z.x * z.y;
        z.x = tmp;
        z = z +(float2)(re, im);
    }

    const float mu = i + 1 - log2(log2(length(z)) / log(2.0));

    int color1 = (int)(mu);
    const int color2 = (color1 + 1) % 16;
    const float t2 = mu - color1;
    const float t1 = 1 - t2;
    color1 %= 16;

    const uchar3 colors1 = colors[color1];
    const uchar3 colors2 = colors[color2];

    uchar r = colors1.s0 * t1 + colors2.s0 * t2;
    uchar g = colors1.s1 * t1 + colors2.s1 * t2;
    uchar b = colors1.s2 * t1 + colors2.s2 * t2;

//    uchar r = i;
//    uchar g = i * (1 - i / mi);
//    uchar b = 255 * (4 / length(z));

    if(r == g && g == b && b == 0) {r = g = b = 255;}


    out[(ix + iy * (int)(input[6]))] = (uchar4)(r, g, b, 255);
//    float t = ix * ix / 1670;
//    for(int i = 0; i<1000; ++i)
//    {
//        t = t * t * t - t * t;
//        t -= 4;
//    }
//    out[ix] = t;
}
