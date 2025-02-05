# version 410

//
//    VS Input (nothing)
//

//
//    VS Output
//
layout(location = 0) out vec4 Color;
layout(location = 1) out vec2 UV;
out gl_PerVertex
{
    vec4 gl_Position;
};

//
//    Siv3D Functions
//
vec4 s3d_Transform2D(const vec2 pos, const vec4 t[2])
{
    return vec4(t[0].zw + (pos.x * t[0].xy) + (pos.y * t[1].xy), t[1].zw);
}

//
//    Constant Buffer
//
layout(std140) uniform VSConstants2D
{
    vec4 g_transform[2];
    vec4 g_colorMul;
};

layout(std140) uniform Wave
{
    float g_t;
};

//
//    Functions
//
float CalcWave(float x, float t)
{
    return sin(x * 0.011 + t*300) * 6
        + sin(x * 0.013 + -t * 1.1*300) * 4
        + sin(x * 0.017 + t * 1.3*300) * 3;
}

void main()
{
    vec2 pos;
    vec4 color;

    uint xi = (gl_VertexID / 6);
    uint i = (gl_VertexID % 6);

    if ((i == 0) || (i == 2) || (i == 3))
    {
        pos.x = (xi * 8);
    }
    else
    {
        pos.x = ((xi + 1) * 8);
    }

    if ((i == 2) || (i == 3) || (i == 5))
    {
        pos.y = 500;
        color = vec4(0.3568, 0.7019, 0.9137, 1.0);
    }
    else
    {
        pos.y = 211 + CalcWave(pos.x, g_t);
        color = vec4(0.3568, 0.7019, 0.9137, 1.0);
    }

    gl_Position = s3d_Transform2D(pos, g_transform);
    Color = color;
    UV = vec2(0, 0);
}
