#include "../Renderer.h"

Renderer::Renderer(unsigned int shaderProgramId, unsigned int positionAttributeLocation, unsigned int textureCoordinateAttributeLocation)//, unsigned int normalAttributeLocation)
{
	m_shaderProgramId = shaderProgramId;
	m_positionAttribLocation = positionAttributeLocation;
	m_textureCoordAttribLocation = textureCoordinateAttributeLocation;
	//m_normalAttribLocation = normalAttributeLocation;

	glGenVertexArrays(1, &m_vao);
	
	//Temps below
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 1.0f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;
	aspect = (float)640 / (float)480;

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void Renderer::RenderModel(const std::string & modelName)
{
	if (auto itr = m_models.find(modelName); itr != m_models.end())
	{
		using namespace std;
		auto& m = itr->second;

		glUseProgram(m_shaderProgramId);

		mvLoc = glGetUniformLocation(m_shaderProgramId, "mv_matrix");
		projLoc = glGetUniformLocation(m_shaderProgramId, "proj_matrix");

		//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		mvMat = vMat * m->GetModelMatrix();

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		pMat = glm::perspective(1.0472f, m_aspectRatio, 0.1f, 1000.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		glBindVertexArray(m_vao);

			// white light
			float globalAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			//float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			//float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			//float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		//TODO: This rendering logic currently assumes there is a material associated with the object.
		//However, it needs to handle the case where no material file(s) is specified.
		for (const auto& pair : m->GetMaterialToVBOMap())
		{
			//current material
			auto material_iter = m_materials->find(pair.first);
			const auto& currentMtl = m_materials->find(pair.first)->second;

			float ambientColor[4] = { currentMtl.GetAmbientColor().x, currentMtl.GetAmbientColor().y , currentMtl.GetAmbientColor().z , 1 };
			float diffuseColor[4] = { currentMtl.GetDiffuseColor().x, currentMtl.GetDiffuseColor().y , currentMtl.GetDiffuseColor().z , 1 };
			float specularColor[4] = { currentMtl.GetSpecularColor().x, currentMtl.GetSpecularColor().y , currentMtl.GetSpecularColor().z , 1 };
			float specularExponent = currentMtl.GetSpecularExponent();
			float* matAmb = ambientColor;
			float* matDif = diffuseColor;
			float* matSpe = specularColor;
			float matShi = specularExponent;

			GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;

			// get the locations of the light and material fields in the shader
			globalAmbLoc = glGetUniformLocation(m_shaderProgramId, "globalAmbient");
			//ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
			//diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
			//specLoc = glGetUniformLocation(renderingProgram, "light.specular");
			//posLoc = glGetUniformLocation(renderingProgram, "light.position");
			mambLoc = glGetUniformLocation(m_shaderProgramId, "material.ambient");
			mdiffLoc = glGetUniformLocation(m_shaderProgramId, "material.diffuse");
			mspecLoc = glGetUniformLocation(m_shaderProgramId, "material.specular");
			mshiLoc = glGetUniformLocation(m_shaderProgramId, "material.shininess");

			//  set the uniform light and material values in the shader
			glProgramUniform4fv(m_shaderProgramId, globalAmbLoc, 1, globalAmbient);
			//glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
			//glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
			//glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
			//glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
			glProgramUniform4fv(m_shaderProgramId, mambLoc, 1, matAmb);
			glProgramUniform4fv(m_shaderProgramId, mdiffLoc, 1, matDif);
			glProgramUniform4fv(m_shaderProgramId, mspecLoc, 1, matSpe);
			glProgramUniform1f(m_shaderProgramId, mshiLoc, matShi);

			const auto& vbos = pair.second;
			glBindBuffer(GL_ARRAY_BUFFER, vbos.m_positionsVBO);
			glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(m_positionAttribLocation);
			glDrawArrays(GL_TRIANGLES, 0, vbos.m_numberOfVertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glActiveTexture(0);
		}

		//Model specific logic here
		//m->Bind(m_positionAttribLocation, m_textureCoordAttribLocation);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		//glDrawArrays(GL_TRIANGLES, 0, m->GetNumberOfVertices());
		//m->Unbind();

		glBindVertexArray(0);
	}
	else
	{
		std::cout << "ERROR: Model named '" << modelName << "' does not exist. Please provide a valid model name." << std::endl;
	}
}

void Renderer::GetModelMatrix(const std::string & modelName, glm::mat4& returnValue)
{
	auto itr = m_models.find(modelName);
	if (itr != m_models.end())
	{
		returnValue = itr->second->GetModelMatrix();
	}
}

const glm::vec3 Renderer::GetModelInitialWorldPosition(const std::string & modelName)
{
	auto itr = m_models.find(modelName);

	if (itr != m_models.end())
	{
		return itr->second->GetInitialWorldPosition();
	}
	return glm::vec3(0, 0, 0);
}

void Renderer::RenderModels()
{	
	glBindVertexArray(m_vao);
	for (auto& m : m_models)
	{
		if (!m.second->IsInitialized())
			continue;

		RenderModel(m.first);
	}
}

void Renderer::AddModel(ModelPtr model)
{
	if (model)
	{	
		if (model->IsInitialized())
		{
			auto result_pair = m_models.insert({ model->GetName(), std::move(model) });

			if (!result_pair.second)
			{
				std::cout << "ERROR: Failed to add model '" << model->GetName() << "' to collection. Model with that name already exists." << std::endl;
			}
		}
		else
		{
			std::cout << "ERROR: Failed to add model '" << model->GetName() << "' to collection. Model pointer is not properly initialized." << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR: Failed to add model '" << model->GetName() << "' to collection. Model pointer is null." << std::endl;
	}
}

void Renderer::SetModelMatrix(const std::string & modelName, glm::mat4&& modelMatrix)
{
	auto itr = m_models.find(modelName);
	if (itr != m_models.end())
	{
		itr->second->SetModelMatrix(std::move(modelMatrix));
	}
	else
	{
		std::cout << "ERROR: Failed to set the model matrix for model named '" << modelName << "'" << std::endl;
	}
}

void Renderer::SetViewMatrix(glm::mat4&& viewMatrix)
{
	vMat = viewMatrix;
}
