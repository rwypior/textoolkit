#version 330

layout (location = 0) in vec3 vertexLocation; 
layout (location = 1) in vec3 vertexNormal; 

out vec3 position;
out vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{          
    mat4 mv = viewMatrix * modelMatrix;
	mat4 mvp = projectionMatrix * mv;
    gl_Position = mvp * vec4(vertexLocation, 1.0);
    position = gl_Position.xyz;

    mat3 normalMatrix = mat3(transpose(inverse(mv)));
    normal = normalize(vec3(vec4(normalMatrix * vertexNormal, 0.0)));
}
