#include "../Model.h"

Model::Model()
{
	m_modelMatrix = glm::mat4(1.0f);
}

void Model::SetupVBOs()
{
	//unsigned int vbo;
	//unsigned int current_vbo_index = 0;
	//glGenBuffers(1, &vbo);
	//m_vbos.push_back(vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vbos[current_vbo_index]);
	//glBufferData(GL_ARRAY_BUFFER, m_positionValues.size() * 4, &m_positionValues[0], GL_STATIC_DRAW);

	//if (HasTexture())
	//{
	//	current_vbo_index++;
	//	glGenBuffers(1, &vbo);
	//	m_vbos.push_back(vbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[current_vbo_index]);
	//	glBufferData(GL_ARRAY_BUFFER, m_textureCoorValues.size() * 4, &m_textureCoorValues[0], GL_STATIC_DRAW);
	//}
	//if (HasNormals())
	//{
	//	current_vbo_index++;
	//	glGenBuffers(1, &vbo);
	//	m_vbos.push_back(vbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[current_vbo_index]);
	//	glBufferData(GL_ARRAY_BUFFER, m_normalValues.size() * 4, &m_normalValues[0], GL_STATIC_DRAW);
	//}

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//m_numVBOs = m_vbos.size();

	for (const auto& pair : m_mtlToFaces)
	{
		VBOCollection vbos;
		const auto& mtlName = pair.first;
		const auto& faceCollection = pair.second;

		if (faceCollection.m_positionValues.size() > 0)
		{
			glGenBuffers(1, &vbos.m_positionsVBO);
			glBindBuffer(GL_ARRAY_BUFFER, vbos.m_positionsVBO);
			glBufferData(GL_ARRAY_BUFFER, faceCollection.m_positionValues.size() * 4, &faceCollection.m_positionValues[0], GL_STATIC_DRAW);
			vbos.m_numberOfVertices = faceCollection.m_positionValues.size() / 3;
		}

		if (faceCollection.m_textureCoorValues.size() > 0)
		{
			glGenBuffers(1, &vbos.m_textureCoordsVBO);
			glBindBuffer(GL_ARRAY_BUFFER, vbos.m_textureCoordsVBO);
			glBufferData(GL_ARRAY_BUFFER, faceCollection.m_textureCoorValues.size() * 4, &faceCollection.m_textureCoorValues[0], GL_STATIC_DRAW);
		}

		if (faceCollection.m_normalValues.size() > 0)
		{
			glGenBuffers(1, &vbos.m_normalsVBO);
			glBindBuffer(GL_ARRAY_BUFFER, vbos.m_normalsVBO);
			glBufferData(GL_ARRAY_BUFFER, faceCollection.m_normalValues.size() * 4, &faceCollection.m_normalValues[0], GL_STATIC_DRAW);
		}

		m_mtlToVBOs.insert({ mtlName, std::move(vbos) });
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::InitializeModelMatrix()
{
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_initialWorldPosition);
}

Model::~Model()
{
	Unbind();
	//glDeleteBuffers(m_numVBOs, &m_vbos[0]);
	for (const auto& pair : m_mtlToVBOs)
	{
		glDeleteBuffers(1, &pair.second.m_positionsVBO);
		glDeleteBuffers(1, &pair.second.m_textureCoordsVBO);
		glDeleteBuffers(1, &pair.second.m_normalsVBO);
	}
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
