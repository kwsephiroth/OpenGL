#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Model.h"
#include "ModelLoader.h"

class Renderer//TODO: Determine if this class should be a Singleton
{
private:
	unsigned int m_shaderProgramId;
	using ModelPtr = std::unique_ptr<Model>;
	//std::vector<ModelPtr> m_models;
	std::unordered_map<std::string, ModelPtr> m_models;//TODO: Create objectName to Model object map
	unsigned int m_positionAttribLocation;
	unsigned int m_textureCoordAttribLocation;
	unsigned int m_normalAttribLocation;
	unsigned int m_colorAttribLocation;
	unsigned int m_vao;

	glm::vec3 m_cameraPosition;
	std::shared_ptr<glm::mat4> m_perspectiveMatrix;
	float m_aspectRatio;
	
	//temporary variable declarations for display	
	float cameraX, cameraY, cameraZ;
	float objLocX, objLocY, objLocZ;
	GLuint mvLoc, projLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat;

public:
	Renderer(unsigned int shaderProgramId, 
			 unsigned int positionAttributeLocation,  
			 unsigned int textureCoordinateAttributeLocation);

	//Renderer(unsigned int shaderProgramId, const VertexAttributeLocations& val);

	~Renderer();

	void RenderModel(const std::string& modelName);
	void RenderModels();
	void AddModel(ModelPtr model);
	void RemoveModel(const std::string& modelName);
	void ClearModels();
	void SetShaderProgramId(unsigned int shaderProgramId) { m_shaderProgramId = shaderProgramId; }
	void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
	void SetModelMatrix(const std::string& modelName, glm::mat4&& modelMatrix);
	void GetModelMatrix(const std::string& modelName, glm::mat4& returnValue);
	const glm::vec3 GetModelInitialWorldPosition(const std::string& modelName);
};