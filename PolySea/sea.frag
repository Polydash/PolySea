#version 150 core

in vec3 normal;

out vec4 out_Color;

void main()
{
	/*vec3 lightDir = vec3(0.0f, -1.0f, 0.0f);
	
	vec3 ambientColor = vec3(0.1f, 0.1f, 0.1f);
	vec3 diffuseColor = vec3(0.8f, 1.0f, 1.0f);

	float ambient = 0.1f;
	float diffuse = clamp(dot(-lightDir, normal), 0.0f, 1.0f);

	vec3 color = ambientColor*ambient + diffuseColor*diffuse*0.5f;*/

	vec3 color = vec3(0.0f, 0.25f, 1.0f);
    out_Color = vec4(color, 1.0f);
}
