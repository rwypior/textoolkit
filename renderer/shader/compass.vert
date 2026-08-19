#version 330 core

layout (location = 0) in vec3 vertexLocation;

out vec3 out_position;

uniform mat4 modelMatrix;
uniform mat4 orthoMatrix;
//uniform mat4 projectionMatrix;

void main()
{
    mat4 mp = orthoMatrix * modelMatrix;
    //mat4 mp = projectionMatrix * modelMatrix;
    gl_Position = mp * vec4(vertexLocation, 1.0);
    out_position = vec3(gl_Position);
}