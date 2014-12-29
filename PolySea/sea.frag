#version 150 core

in vec3 normal;

out vec4 out_Color;

void main()
{
	//TODO : Set it as uniform
	vec3 lightDir = vec3(0.0f, -1.0f, 0.0f);
	float diffuseContrast = 9.0f;
	vec3 diffuseColor = vec3(0.2f, 0.2f, 0.2f);
	vec3 ambientColor = vec3(0.0f, 0.25f, 1.0f);

	//Compute diffuse parameter
	float diffuse = clamp(dot(-lightDir, normal), 0.0f, 1.0f);
	diffuse = (diffuse - (1.0f - 1.0f/diffuseContrast)) * diffuseContrast;

	vec3 color = ambientColor + diffuse * diffuseColor;

	out_Color = vec4(color, 1.0f);
}
