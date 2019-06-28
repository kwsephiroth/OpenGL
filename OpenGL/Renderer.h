#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "Model.h"
#include "ModelLoader.h"

class Renderer//TODO: Determine if this class should be a Singleton
{
private:
	unsigned int m_shaderProgramId;
	std::vector<std::unique_ptr<Model>> m_models;
	unsigned int m_positionAttribLocation;
	unsigned int m_textureCoordAttribLocation;
	unsigned int m_normalAttribLocation;
	unsigned int m_colorAttribLocation;
	unsigned int m_vao;

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

	void RenderModel(const std::string& objectName);
	void RenderModels();
	void AddModel(std::unique_ptr<Model> model);
	void RemoveModel(const std::string& objectName);
	void ClearModels();
	void SetShaderProgramId(unsigned int shaderProgramId);
};