#pragma once

#include <GLM\glm.hpp>

#include "Shader.h"

typedef unsigned int uint;

const float M_PI = 3.14159265358979323846f;
const uint WAVE_NUM = 16;

struct WaveSettings
{
	float amplitude;
	float waveLength;
	float speed;
	glm::vec2 direction;
};

class Sea
{
private:
	uint m_size;
	uint m_faceNum;
	uint m_waveNum;
	Shader *m_pShader;
	float  *m_pVertices;
	WaveSettings m_waves[WAVE_NUM];

public:
	Sea(uint size, uint faceNum);
	~Sea();

	bool Init();
	void Draw(const glm::mat4 &mvp, float time);

private:
	void SetWaves();
	glm::vec2 GetDirectionVec(float angle) const;
};

