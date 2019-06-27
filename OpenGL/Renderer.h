#pragma once
#include <GL/glew.h>
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
	std::vector<std::unique_ptr<Model>> m_Models;

public:
	Renderer(unsigned int shaderProgramId) : m_shaderProgramId(shaderProgramId) {};
	~Renderer() = default;

	void RenderModel(const std::string& objectName);
	void RenderModels();
	void AddModel(std::unique_ptr<Model> model);
	void RemoveModel(const std::string& objectName);
	void ClearModels();
	void SetShaderProgramId(unsigned int shaderProgramId);
};