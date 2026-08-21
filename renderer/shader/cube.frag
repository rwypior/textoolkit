#version 330

in vec3 position;
in vec3 normal;
in vec3 uv;

out vec3 outColor;

uniform bool lightingenabled = true;

uniform vec3 lightDirection;
uniform vec3 globalLightColor;
uniform vec3 lightColor;

uniform samplerCube texColor;

void main()
{
	vec3 lightDir = normalize(-lightDirection);
	float reflection = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * reflection;
	
	outColor = texture(texColor, uv).rgb;

	//outColor = color * globalLightColor + lightColor;
	//outColor = normal;
	//outColor = lightColor;
}
