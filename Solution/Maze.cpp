#include "Maze.h"

#include <glm/glm.hpp>
Maze::Maze()
{
	this->imageData = nullptr;
	this->width = 0;
	this->height = 0;
	this->numComponents = 0;
	GLuint m_texture = 0;

	this->tbo = 0;
	this->vbo = 0;
	this->vao = 0;
}

Maze::~Maze()
{
	stbi_image_free(imageData);
	//glDeleteBuffers(1, &this->tbo);
}

int Maze::GetHeight()
{
	return this->height;
}

int Maze::GetWidth()
{
	return this->width;
}

void Maze::initiateBuffers(GLuint programID)
{
	// create a buffer to hold the results of the transform feedback process.
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	// allocate space (no data)
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * 18 * 64 * 64 + sizeof(glm::vec2) * 12 * 64 * 64,		// 884,736 bytes <1Mb
		NULL,							// no data passed
		GL_DYNAMIC_COPY);

	// VAO to draw points
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	GLint positionAttrib = glGetAttribLocation(programID, "inPosition");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint texcoordAttrib = glGetAttribLocation(programID, "inTexCoords");
	glEnableVertexAttribArray(texcoordAttrib);
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)12);

	// create and bind transform feedback object and buffer to write to.
	glGenTransformFeedbacks(1, &this->tbo);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->vbo);
}

// Loading the .bmp file and returning the width,height and pointer to the first pixel in the file.
void Maze::LoadMaze(const std::string & fileName)
{
	this->imageData = stbi_load(fileName.c_str(), &this->width, &this->height, &this->numComponents, 3);

	if (this->imageData == NULL)
	{
		std::cerr << "Loading failed for texture: " << fileName << std::endl;
	}

	glGenTextures(1, &this->m_texture);
	glBindTexture(GL_TEXTURE_2D, this->m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->imageData);
}

unsigned char Maze::readPixel(unsigned int x, unsigned int y)
{
	return this->imageData[x*y*this->numComponents];
}

void Maze::Draw()
{
	// Perform feedback transform
	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tbo);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->vbo);

	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArrays(GL_POINTS, 0, this->width * this->height);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	//glFlush();
}

void Maze::DrawMaze()
{
	//glm::vec3 feedback[64 * 64 * 3];
	//glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	//
	//for (int i = 0; i < (64 * 64 * 3); i++)
	//{
	//	std::cout << "x: " << feedback[i].x;
	//	std::cout << "     y: " << feedback[i].y;
	//	std::cout << "     z: " << feedback[i].z << std::endl;
	//}

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK_BUFFER, this->tbo);
	glDrawTransformFeedback(GL_TRIANGLES, this->tbo);
}

void Maze::BindTexture(unsigned int textureUnit)
{
	if (textureUnit  >= 0 && textureUnit <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}
