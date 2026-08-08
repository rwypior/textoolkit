#version 330 core

in GEOM_OUT
{
    vec3 color;
} frag_in;

out vec4 FragColor;

void main()
{
    FragColor = vec4(frag_in.color, 1.0);
}