#pragma once
#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Vertex
{
	glm::vec3 positions;
	glm::vec3 color;
	glm::vec2 textureCoordinates;
	glm::vec3 normal;
};

class Model
{
private:
	
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_textureCoordinates;
	std::vector<glm::vec3> m_normals;

	/*struct Triangle
	{
		std::vector<unsigned int> m_vertexIndices;
		std::vector<unsigned int> m_textureCoordinateIndices;
		std::vector<unsigned int> m_normalIndices;
	};

	std::vector<Triangle> m_triangles;
	std::vector<Vertex> m_vertices;*/
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