#version 330

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 outColor;

uniform bool lightingenabled = true;

uniform vec3 lightDirection;
uniform vec3 globalLightColor;
uniform vec3 lightColor;

uniform sampler2D texColor;

void main()
{
	vec2 texsize = vec2(textureSize(texColor, 0));
	vec2 dx = dFdx(uv * texsize);
	vec2 dy = dFdy(uv * texsize);
	float d = max(length(dx), length(dy));
	float lod = max(0., 1.0 * log(d) / log(2.0)); // Play with the values to make mipmaps appear earlier/later

	vec3 lightDir = normalize(-lightDirection);
	float reflection = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * reflection;

	outColor = textureLod(texColor, uv, lod).rgb;

	//outColor = color * globalLightColor + lightColor;
	//outColor = normal;
	//outColor = lightColor;
}
