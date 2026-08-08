#version 330 core

layout (location = 0) in vec3 vertexLocation;

out VS_OUT
{
    vec3 position;
} vs_out;


void main()
{
    gl_Position = mv * vec4(vertexLocation, 1.0); 
    vs_out.position = gl_Position;
}