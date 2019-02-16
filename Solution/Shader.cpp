#include "Shader.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string fileName);


Shader::Shader()
{
	program = glCreateProgram();
	this->NUM_OF_SHADERS = 0;
}

void Shader::Bind()
{
	glUseProgram(program);
}

void Shader::unBind()
{
	glUseProgram(0);
}

void Shader::Update(const Transform & transform, const Camera& camera)
{
	glm::mat4 modelT = camera.getViewProjection() * transform.getWorldMatrix();
	glm::mat4 modelW = transform.getWorldMatrix();


	// Skicka matriserna till GPU'n
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &modelT[0][0]);
	glUniformMatrix4fv(uniforms[WORLD_U], 1, GL_FALSE, &modelW[0][0]);
}

void Shader::initiateShaders(bool color)
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	glBindAttribLocation(program, 0, "position");

	if (color == true)
	{
		glBindAttribLocation(program, 1, "color");
	}
	else
	{
		glBindAttribLocation(program, 1, "texCoord");
	}	

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	// Berättar för GPU'n vad namnet på inkommande variabel är.
	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transformationMatrix");
	uniforms[WORLD_U] = glGetUniformLocation(program, "WorldMatrix");
}

void Shader::initiateMazeShader()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	const GLchar * feedbackVarying[] = { "outPosition", "outTexCoords" };
	glTransformFeedbackVaryings(this->program, 2, feedbackVarying, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glUseProgram(this->program);
}

void Shader::initiateTestShader()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
}

void Shader::validateShaders()
{
	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
}


Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(program);
}

void Shader::sendInt(const char *name, int value)
{
	glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::sendFloat(const char * name, float value)
{
	glUniform1f(glGetUniformLocation(this->program, name), value);
}

void Shader::sendVec3(const char * name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(this->program, name), x, y, z);
}
/*
void Shader::sendMat4(const char *name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, &mat[0][0]);
}*/
void Shader::setMat4(const std::string &name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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

GLuint *Shader::getProgram()
{
	return &this->program;
}