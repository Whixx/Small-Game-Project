#include "Shader.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string fileName);


Shader::Shader()
{
	this->program = glCreateProgram();
	this->NUM_OF_SHADERS = 0;
}

void Shader::Bind()
{
	glUseProgram(this->program);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

void Shader::InitiateShaders(bool color)
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
		glAttachShader(this->program, this->shaders[i]);

	glBindAttribLocation(this->program, 0, "position");

	if (color == true)
	{
		glBindAttribLocation(this->program, 1, "color");
	}
	else
	{
		glBindAttribLocation(this->program, 1, "texCoord");
	}	

	glLinkProgram(this->program);
	CheckShaderError(this->program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
}

void Shader::initiateMazeShader()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	const GLchar * feedbackVarying[] = { "outPosition", "outTexCoords", "outNormal", "outTangent" };
	glTransformFeedbackVaryings(this->program, 4, feedbackVarying, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glUseProgram(this->program);
}

void Shader::ValidateShaders()
{
	glValidateProgram(this->program);
	CheckShaderError(this->program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
}


Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
	{
		glDetachShader(this->program, this->shaders[i]);
		glDeleteShader(this->shaders[i]);
	}

	glDeleteProgram(program);
}

int Shader::GetUniformLocation(const std::string & name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int location = glGetUniformLocation(this->program, name.c_str());
	if (location == -1)
	{
		std::cout << "Uniform " << name << " was not found" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SendInt(const char *name, int value)
{
	glUniform1i(this->GetUniformLocation(name), value);
}

void Shader::SendFloat(const char * name, float value)
{
	glUniform1f(this->GetUniformLocation(name), value);
}

void Shader::SendVec3(const char * name, float x, float y, float z)
{
	glUniform3f(this->GetUniformLocation(name), x, y, z);
}

void Shader::SendMat4(const char * name, const glm::mat4 & mat)
{
	glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SendCameraLocation(Camera * camera)
{
	glUniform3f(this->GetUniformLocation("cameraPos"), camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

std::string LoadShader(const std::string fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	file.close();

	return output;
}

GLuint Shader::CreateShader(const std::string & fileName, GLenum shaderType)
{
	std::string shaderCode = LoadShader(fileName);

	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed!" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = shaderCode.c_str();
	shaderSourceStringLengths[0] = shaderCode.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

	shaders[this->NUM_OF_SHADERS++] = shader;
	return shader;
}

GLuint *Shader::GetProgram()
{
	return &this->program;
}