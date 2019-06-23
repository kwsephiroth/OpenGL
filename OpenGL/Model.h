#pragma once
#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

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
	float position[3];
	float textureCoordinates[2];
	float normal[3];
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
	unsigned int m_ibo;
	bool m_initialized = false;
	bool m_smoothed = false;

	void InitializeVertices();
	Model() = default;

public:
	
	~Model() = default;
	
	void Bind();
	void Unbind();

	inline bool IsInitialized() const { return m_initialized; }

	friend class ModelLoader;
};