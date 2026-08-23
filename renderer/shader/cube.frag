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
	vec3 uvnorm = normalize(uv);

	float texsize = textureSize(texColor, 0).x;
	vec3 dx = dFdx(uvnorm * texsize);
	vec3 dy = dFdy(uvnorm * texsize);
	float d = max(length(dx), length(dy));
	float lod = max(0., 1.0 * log(d) / log(2.0)); // Play with the values to make mipmaps appear earlier/later

	vec3 lightDir = normalize(-lightDirection);
	float reflection = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * reflection;
	
	outColor = textureLod(texColor, uvnorm, lod).rgb;

	//outColor = color * globalLightColor + lightColor;
	//outColor = normal;
	//outColor = lightColor;
}
