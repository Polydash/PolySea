#include "Shader.h"

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) : 
m_vertexID(0),
m_fragmentID(0),
m_programID(0),
m_vertexSource(vertexSource),
m_fragmentSource(fragmentSource)
{
}

Shader::~Shader()
{
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}

bool Shader::Load()
{
    //Clear from previous shader if necessary
    if(glIsShader(m_vertexID) == GL_TRUE)
        glDeleteShader(m_vertexID);

    if(glIsShader(m_fragmentID) == GL_TRUE)
        glDeleteShader(m_fragmentID);

    if(glIsProgram(m_programID) == GL_TRUE)
        glDeleteProgram(m_programID);


    //Shader compilation
    if(!Compile(m_vertexID, GL_VERTEX_SHADER, m_vertexSource))
        return false;

	if(!Compile(m_geometryID, GL_GEOMETRY_SHADER, "sea.geom"))
		return false;

    if(!Compile(m_fragmentID, GL_FRAGMENT_SHADER, m_fragmentSource))
        return false;

    //Create
    m_programID = glCreateProgram();

    //Attach
    glAttachShader(m_programID, m_vertexID);
	glAttachShader(m_programID, m_geometryID);
    glAttachShader(m_programID, m_fragmentID);

    //Bind
    glBindAttribLocation(m_programID, 0, "in_Vertex");
    glBindAttribLocation(m_programID, 1, "in_Normal");

    //Link
    glLinkProgram(m_programID);

    //Error checking
    GLint linkState(0);
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkState);

    if(linkState != GL_TRUE)
    {
        GLint errSize(0);
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &errSize);

        char *errMsg = new char[errSize + 1];

        glGetProgramInfoLog(m_programID, errSize, &errSize, errMsg);
        errMsg[errSize] = '\0';

        std::cout << errMsg << std::endl;

        delete[] errMsg;
        glDeleteProgram(m_programID);

        return false;
    }

    return true;
}


bool Shader::Compile(GLuint &shader, GLenum type, const std::string &sourceFile)
{
    //Create shader
    shader = glCreateShader(type);
    if(!shader)
    {
        std::cout << "Error : shader type \"" << type << "\" does not exist" << std::endl;
        return false;
    }

	//Get source file
    std::ifstream file(sourceFile.c_str());
    if(!file)
    {
        std::cout << "Error : File \"" << sourceFile << "\" could not be found" << std::endl;
        glDeleteShader(shader);
        return false;
    }

	//Read code from file
    std::string line;
    std::string sourceCode;

    while(getline(file, line))
        sourceCode += line + '\n';

	//Close source file
    file.close();

	//Set shader code
    const GLchar* src = sourceCode.c_str();
    glShaderSource(shader, 1, &src, 0);

	//Compilation
    glCompileShader(shader);

	//Error checking
    GLint compileState(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileState);

    if(compileState != GL_TRUE)
    {
        GLint errSize(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errSize);

        char *errMsg = new char[errSize + 1];
        glGetShaderInfoLog(shader, errSize, &errSize, errMsg);
        errMsg[errSize] = '\0';

        std::cout << errMsg << std::endl;

        delete[] errMsg;
        glDeleteShader(shader);

        return false;
    }

    return true;
}

GLuint Shader::GetProgramID() const
{
    return m_programID;
}
