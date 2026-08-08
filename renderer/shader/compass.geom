#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 position;
} gs_in[];

out GEOM_OUT
{
    vec3 color;
} gs_out;
  
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 colors;

void main()
{
    vec4 center = projectionMatrix * gl_in[0].gl_Position;

    gl_Position = center;
    gs_out.color = colors[0];
    EmitVertex();
    gl_Position = projectionMatrix * modelMatrix * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0));
    EmitVertex();
    
    gl_Position = center;
    gs_out.color = colors[1];
    EmitVertex();
    gl_Position = projectionMatrix * modelMatrix * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0));
    EmitVertex();

    gl_Position = center;
    gs_out.color = colors[2];
    EmitVertex();
    gl_Position = projectionMatrix * modelMatrix * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0));
    EmitVertex();

    EndPrimitive();
}  