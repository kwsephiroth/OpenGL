#pragma once
#include <glm\glm.hpp>
#include <string>

struct MaterialProperties
{
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularExponent;
};

struct SpecularComponent
{
	glm::vec3 specularColor;
	float specularExponent;
};

class Material
{
private:
	std::string m_name;
	MaterialProperties m_properties;

public:
	Material(std::string name, MaterialProperties properties) : m_name(name), m_properties(properties) {}
	Material(std::string name) : m_name(name) {}
	~Material() = default;

	const glm::vec3& GetAmbientColor() const { return m_properties.ambientColor; }
	const glm::vec3& GetDiffuseColor() const { return m_properties.diffuseColor; }
	const glm::vec3& GetSpecularColor() const { return m_properties.specularColor; }
	const float GetSpecularExponent() const{ return m_properties.specularExponent; }

	friend class MaterialLoader;
};

