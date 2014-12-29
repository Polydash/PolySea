#include <stdlib.h>
#include <math.h>
#include <string>
#include <GLM\gtc\type_ptr.hpp>

#include "Sea.h"

Sea::Sea(uint size, uint faceNum) :
m_size(size),
m_faceNum(faceNum),
m_pShader(NULL),
m_pVertices(NULL)
{
}

Sea::~Sea()
{
	//Free allocated memory
	if(m_pVertices)
		delete m_pVertices;

	if(m_pShader)
		delete m_pShader;
}

bool Sea::Init()
{
	//Init shader
	m_pShader = new Shader("sea.vert", "sea.frag");
	if(!m_pShader->Load())
	{
		return false;
	}

	//Allocate vertices
	m_pVertices = new float[m_faceNum * m_faceNum * 18];

	//Fill vertices informations
	float interval = ((float)m_size) / m_faceNum;
	float halfSize = ((float)m_size) / 2.0f;
	for(uint i = 0; i < m_faceNum; ++i)
	{
		for(uint j = 0; j < m_faceNum; ++j)
		{
			//Quad vertices (two triangles)

			uint it = (i*m_faceNum + j) * 18;

			//First triangle
			m_pVertices[it]		= -halfSize + i*interval;
			m_pVertices[it + 1] = 0.0f;
			m_pVertices[it + 2] = -halfSize + j*interval;

			m_pVertices[it + 3] = -halfSize + i*interval;
			m_pVertices[it + 4] = 0.0f;
			m_pVertices[it + 5] = -halfSize + (j + 1)*interval;

			m_pVertices[it + 6] = -halfSize + (i + 1)*interval;
			m_pVertices[it + 7] = 0.0f;
			m_pVertices[it + 8] = -halfSize + j*interval;

			//Second triangle
			m_pVertices[it + 9]  = -halfSize + (i + 1)*interval;
			m_pVertices[it + 10] = 0.0f;
			m_pVertices[it + 11] = -halfSize + j*interval;

			m_pVertices[it + 12] = -halfSize + i*interval;
			m_pVertices[it + 13] = 0.0f;
			m_pVertices[it + 14] = -halfSize + (j + 1)*interval;

			m_pVertices[it + 15] = -halfSize + (i + 1)*interval;
			m_pVertices[it + 16] = 0.0f;
			m_pVertices[it + 17] = -halfSize + (j + 1)*interval;
		}
	}

	SetWaves();
	
	return true;
}

void Sea::Draw(const glm::mat4 &mvp, float time)
{
	//Bind shader
	glUseProgram(m_pShader->GetProgramID());

		//Render
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_pVertices);
		glEnableVertexAttribArray(0);

		glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), "time"), time);
		glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		for(uint i = 0; i < m_waveNum; ++i)
		{
			std::string arrayIt = "[" + std::to_string(i) + "]";

			std::string amplitudeStr("amplitude" + arrayIt);
			std::string waveFreqStr("waveFreq" + arrayIt);
			std::string phaseStr("phase" + arrayIt);
			std::string directionStr("direction" + arrayIt);

			float waveFreq = 2.0f * M_PI / m_waves[i].waveLength;
			float phase = m_waves[i].speed * 2.0f * M_PI / m_waves[i].waveLength;

			glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), amplitudeStr.c_str()), m_waves[i].amplitude);
			glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), waveFreqStr.c_str()), waveFreq);
			glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), phaseStr.c_str()), phase);
			glUniform2f(glGetUniformLocation(m_pShader->GetProgramID(), directionStr.c_str()), m_waves[i].direction.x, m_waves[i].direction.y);
		}

		glDrawArrays(GL_TRIANGLES, 0, 6 * m_faceNum * m_faceNum);

		glDisableVertexAttribArray(0);

	glUseProgram(0);
}

void Sea::SetWaves()
{
	m_waveNum = 4;
	m_waves[0] = {0.1f, 3.0f, 0.9f, GetDirectionVec(-M_PI/6.0f)};
	m_waves[1] = {0.2f, 8.0f, 0.8f, GetDirectionVec(2*M_PI/3.0f)};
	m_waves[2] = {0.5f, 16.0f, 3.0f, GetDirectionVec(M_PI/3.0f)};
	m_waves[3] = {0.7f, 32.0f, 4.0f, GetDirectionVec(-M_PI/6.0f)};

	/*
	for(uint i = 0; i < m_waveNum; ++i)
	{
		m_waves[i].amplitude = 1.0f / (i + 1);
		m_waves[i].waveLength = 16 * M_PI / (i + 1);
		m_waves[i].speed = 1.0f + i;

		float angle = (rand() / ((float)RAND_MAX)) * (M_PI / 1.5f) - (M_PI / 3.0f);
		m_waves[i].direction = glm::vec2(cos(angle), sin(angle));
	}*/
}


glm::vec2 Sea::GetDirectionVec(float angle) const
{
	return glm::vec2(cos(angle), sin(angle));
}