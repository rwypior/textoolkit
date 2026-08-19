#version 330

in vec3 position;
in vec3 normal;

out vec3 outColor;

uniform bool lightingenabled = true;
uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 selectionColor = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 lightDirection;
uniform vec3 globalLightColor;
uniform vec3 lightColor;

void main()
{
	vec3 lightDir = normalize(-lightDirection);
	float reflection = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * reflection;
	
	if (lightingenabled)
		outColor = globalLightColor + color * diffuse;
	else
		outColor = color;

	outColor *= selectionColor;
	//outColor = color * globalLightColor + lightColor;
	//outColor = normal;
	//outColor = lightColor;
}
