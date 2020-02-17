#include "../Model.h"

Model::Model()
{
	m_modelMatrix = glm::mat4(1.0f);
}

void Model::SetupVBOs()
{
	unsigned int vbo;
	unsigned int current_vbo_index = 0;
	glGenBuffers(1, &vbo);
	m_vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, m_positionValues.size() * 4, &m_positionValues[0], GL_STATIC_DRAW);

	if (HasTexture())
	{
		current_vbo_index++;
		glGenBuffers(1, &vbo);
		m_vbos.push_back(vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[current_vbo_index]);
		glBufferData(GL_ARRAY_BUFFER, m_textureCoorValues.size() * 4, &m_textureCoorValues[0], GL_STATIC_DRAW);
	}
	if (HasNormals())
	{
		current_vbo_index++;
		glGenBuffers(1, &vbo);
		m_vbos.push_back(vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[current_vbo_index]);
		glBufferData(GL_ARRAY_BUFFER, m_normalValues.size() * 4, &m_normalValues[0], GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_numVBOs = m_vbos.size();
}

void Model::InitializeModelMatrix()
{
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_initialWorldPosition);
}

Model::~Model()
{
	Unbind();
	glDeleteBuffers(m_numVBOs, &m_vbos[0]);
}

void Model::Bind(unsigned int positionAttribLocation, unsigned int textureAttribLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribLocation);

	if (HasTexture())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
		glVertexAttribPointer(textureAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(textureAttribLocation);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
}

void Model::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glActiveTexture(0);
}

void Model::ResetWorldPosition()
{
	InitializeModelMatrix();
}
