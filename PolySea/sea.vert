#version 150 core

#define WAVE_NUM 4

in vec3 in_Vertex;
out vec3 worldPos;
//out vec3 normal;

uniform float time;
uniform mat4 mvp;

uniform float amplitude[WAVE_NUM];
uniform float waveFreq[WAVE_NUM];
uniform float phase[WAVE_NUM];
uniform vec2  direction[WAVE_NUM];

float waveHeight(int i, float x, float z)
{
	return amplitude[i] * sin(dot(direction[i], vec2(x, z)) * waveFreq[i] + time * phase[i]);
}

float waveDx(int i, float x, float z)
{
    float A = amplitude[i] * direction[i].x * waveFreq[i];
    return A * cos(dot(direction[i], vec2(x, z)) * waveFreq[i] + time * phase[i]);
}

float waveDy(int i, float x, float z)
{
    float A = amplitude[i] * direction[i].y * waveFreq[i];
    return A * cos(dot(direction[i], vec2(x, z)) * waveFreq[i] + time * phase[i]);
}

void main()
{
	vec3 position = vec3(in_Vertex.x, 0.0, in_Vertex.z);
	
	for(int i=0; i<WAVE_NUM; ++i)
	{
		position.y += waveHeight(i, position.x, position.z);
	}

	worldPos = position;
    gl_Position = mvp * vec4(position, 1.0);
}
