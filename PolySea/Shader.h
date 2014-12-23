#pragma once

#include <gl\glew.h>
#include <iostream>
#include <string>
#include <fstream>

class Shader
{
private :

	GLuint m_vertexID;
	GLuint m_geometryID;
	GLuint m_fragmentID;
	GLuint m_programID;

	std::string m_vertexSource;
	std::string m_fragmentSource;

public :

    Shader(const std::string &vertexSource, const std::string &fragmentSource);
    ~Shader();

    bool Load();
	bool Compile(GLuint &shader, GLenum type, const std::string &sourceFile);
    GLuint GetProgramID() const;
};