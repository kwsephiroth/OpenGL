#include "../Model.h"

Model::Model()
{
}

void Model::SetupVBOs()
{
	glGenBuffers(m_numVBOs, m_vbos);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, m_positionValues.size() * 4, &m_positionValues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, m_textureCoorValues.size() * 4, &m_textureCoorValues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, m_normalValues.size() * 4, &m_normalValues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::~Model()
{
	Unbind();
	glDeleteBuffers(m_numVBOs, m_vbos);
}

void Model::Bind(unsigned int positionAttribLocation, unsigned int textureAttribLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribLocation);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glVertexAttribPointer(textureAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureAttribLocation);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Model::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glActiveTexture(0);
}
