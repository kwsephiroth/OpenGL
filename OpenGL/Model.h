#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

/*struct Vertex
{
	glm::vec3 positions;
	glm::vec3 color;
	glm::vec2 textureCoordinates;
	glm::vec3 normal;
};*/

/*struct Position3D
{
	float x;
	float y;
	float z;
};

struct Position2D
{
	float x;
	float y;
};

struct TextureCoordinate
{
	float u;
	float v;
};*/


struct Vertex
{
	std::vector<float> position = { 0,0,0 };
	std::vector<float> color = { 0, 0, 0 };
	std::vector<float> textureCoordinates = { 0,0 };
	std::vector<float> normal = { 0,0,0 };

	/*Vertex()
	{
		position.resize(3);
		textureCoordinates.resize(2);
		normal.resize(3);
	}*/
};

enum class VertexAttributeType
{
	Position,
	Color,
	TextureCoordinate,
	Normal
};

struct VertexAttribute
{
	VertexAttributeType type;
	unsigned int location;
};

class Model
{
private:
	
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec2> m_textureCoordinates;
	std::vector<glm::vec3> m_normals;
	std::vector<Vertex> m_vertices;
	std::string m_name;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_textureID;

	bool m_initialized = false;
	bool m_smoothed = false;

	size_t m_positionOffset;
	size_t m_colorOffset;
	size_t m_textureOffset;
	size_t m_normalOffset;
	size_t m_positionSize;
	size_t m_colorSize;
	size_t m_textureCoorSize;
	size_t m_normalSize;
	size_t m_vertexSize;

	//TODO: Texture m_texture;

	Model();
	void SetupVAO();

public:
	
	~Model();
	
	void Bind();
	void Unbind();

	inline unsigned int GetTextureID() const { return m_textureID; }
	inline bool IsInitialized() const { return m_initialized; }
	inline unsigned int GetNumberOfVertices () const { return m_vertices.size(); }
	inline size_t GetPositionOffset() const { return m_positionOffset; }
	inline size_t GetColorOffset() const { return m_colorOffset; }
	inline size_t GetTextureOffset() const { return m_textureOffset; }
	inline size_t GetNormalOffset() const { return m_normalOffset; }
	inline size_t GetPositionSize() const { return m_positionSize; }
	inline size_t GetColorSize() const { return m_colorSize; }
	inline size_t GetTextureCoordinateSize() const { return m_textureCoorSize; }
	inline size_t GetNormalSize() const { return m_normalSize; }
	inline size_t GetStride() const { return m_vertexSize; }

	friend class ModelLoader;
};