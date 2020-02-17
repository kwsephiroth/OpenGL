#include "../Renderer.h"

Renderer::Renderer(unsigned int shaderProgramId, unsigned int positionAttributeLocation, unsigned int textureCoordinateAttributeLocation)//, unsigned int normalAttributeLocation)
{
	m_shaderProgramId = shaderProgramId;
	m_positionAttribLocation = positionAttributeLocation;
	m_textureCoordAttribLocation = textureCoordinateAttributeLocation;
	//m_normalAttribLocation = normalAttributeLocation;

	glGenVertexArrays(1, &m_vao);
	
	//Temps below
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 1.5f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;
	aspect = (float)640 / (float)480;
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void Renderer::RenderModel(const std::string & modelName)
{
	auto itr = m_models.find(modelName);

	if (itr != m_models.end())
	{
		using namespace std;
		auto& m = itr->second;
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shaderProgramId);

		mvLoc = glGetUniformLocation(m_shaderProgramId, "mv_matrix");
		projLoc = glGetUniformLocation(m_shaderProgramId, "proj_matrix");

		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		mvMat = vMat * m->GetModelMatrix();

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		pMat = glm::perspective(1.0472f, m_aspectRatio, 0.1f, 1000.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		glBindVertexArray(m_vao);

		//Model specific logic here
		m->Bind(m_positionAttribLocation, m_textureCoordAttribLocation);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, m->GetNumberOfVertices());
		m->Unbind();

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
	using namespace std;
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shaderProgramId);

	mvLoc = glGetUniformLocation(m_shaderProgramId, "mv_matrix");
	projLoc = glGetUniformLocation(m_shaderProgramId, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	
	pMat = glm::perspective(1.0472f, m_aspectRatio, 0.1f, 1000.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	
	glBindVertexArray(m_vao);
	for (auto& m : m_models)
	{
		if (!m.second->IsInitialized())
			continue;

		mvMat = vMat * m.second->GetModelMatrix();	
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		m.second->Bind(m_positionAttribLocation, m_textureCoordAttribLocation);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, m.second->GetNumberOfVertices());
		m.second->Unbind();	
	}
	glBindVertexArray(0);
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
