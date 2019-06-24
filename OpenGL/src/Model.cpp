#include "../Model.h"

Model::Model()
{
	auto typeSize = sizeof(float);
	m_positionSize = 3;
	m_colorSize = 3;
	m_textureCoorSize = 2;
	m_normalSize = 3;
	m_positionOffset = 0;
	m_colorOffset = typeSize * 3;
	m_textureOffset = typeSize * 6;
	m_normalOffset = typeSize * 8;
	m_vertexSize = sizeof(Vertex);
}

void Model::SetupVAO()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::~Model()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Model::Bind()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void Model::Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
