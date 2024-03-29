#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>
#include <unordered_map>

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

struct VertexAttributeLocations
{
	unsigned int position;
	unsigned int color;
	unsigned int texture;
	unsigned int normal;
};

struct FaceCollection
{
	//Values in correct face (triangle) order
	std::vector<float> m_positionValues;//Axis values stored separately one element at a time
	std::vector<float> m_textureCoorValues;
	std::vector<float> m_normalValues;
};

struct VBOCollection
{
	unsigned int m_positionsVBO;
	unsigned int m_textureCoordsVBO;
	unsigned int m_normalsVBO;
	unsigned int m_numberOfVertices;
};

class Model
{
private:
	//Values cached from parsing file
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec2> m_textureCoordinates;
	std::vector<glm::vec3> m_normals;

	//Values in correct face (triangle) order
	std::vector<float> m_positionValues;//Axis values stored separately one element at a time
	std::vector<float> m_textureCoorValues;
	std::vector<float> m_normalValues;
	
	std::string m_name;
	unsigned int m_stride;
	unsigned int m_textureID;
	unsigned int m_numVBOs;
	std::vector<unsigned int> m_vbos;

	bool m_initialized = false;
	bool m_smoothed = false;

	glm::mat4 m_modelMatrix;
	glm::vec3 m_initialWorldPosition = { 0,0,0 };

    std::unordered_map<std::string, VBOCollection> m_mtlToVBOs;
	//TODO: Texture m_texture;

	std::unordered_map<std::string, FaceCollection> m_mtlToFaces;
	//std::unordered_map<std::string, VBOCollection> m_mtlToVBOs;

	Model();
	void SetupVBOs();
	void InitializeModelMatrix();

public:
	~Model();

	void Bind(unsigned int positionAttribLocation, unsigned int textureAttribLocation);
	void Unbind();

	inline unsigned int GetTextureID() const { return m_textureID; }
	inline bool IsInitialized() const { return m_initialized; }
	inline unsigned int GetNumberOfVertices() const { return m_positions.size(); }
	inline std::string GetName() const { return m_name; }
	bool HasTexture() { return (!m_textureCoordinates.empty() ? true : false); }
	bool HasNormals() { return (!m_normals.empty() ? true : false); }

	void SetModelMatrix(glm::mat4&& modelMatrix) { m_modelMatrix = std::move(modelMatrix); }
	const glm::mat4& GetModelMatrix() const { return m_modelMatrix; }
	const glm::vec3& GetInitialWorldPosition() const { return m_initialWorldPosition; }
	const std::unordered_map<std::string, VBOCollection>& GetMaterialToVBOMap() const { return m_mtlToVBOs; };
	void ResetWorldPosition();
	void ResetWorldPosition(glm::vec3 worldPosition);
	
	

	friend class ModelLoader;
	friend class Renderer;
};