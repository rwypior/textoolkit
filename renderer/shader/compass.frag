#version 330 core

out vec4 out_color;

uniform mat3 colors;

void main()
{
    vec3 color = colors[gl_PrimitiveID];
    out_color = vec4(color, 1.0);
}