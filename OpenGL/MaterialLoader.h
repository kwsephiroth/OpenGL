#pragma once
#include "Material.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>

class MaterialLoader
{
private:
	MaterialLoader() = default;
	static void SplitLineByDelimeter(const std::string& line, char delimiter, std::queue < std::string>& destination);//TODO: Move this function to a common, shared location
	
public:
	using MaterialsMap = std::unordered_map<std::string, Material>;

	static std::unique_ptr<MaterialsMap> LoadMaterialFromMtlFile(const std::string& mtlFilePath);
};

static std::ostream& operator << (std::ostream& os, const Material& mtl)
{
	os << "Material Name: " << mtl.m_name << std::endl;
	os << "Ambient Color: " << std::setprecision(6) << std::fixed << mtl.m_properties.ambientColor.x << " " << mtl.m_properties.ambientColor.y << " " << mtl.m_properties.ambientColor.z << std::endl;
	os << "Diffuse Color: " << mtl.m_properties.diffuseColor.x << " " << mtl.m_properties.diffuseColor.y << " " << mtl.m_properties.diffuseColor.z << std::endl;
	os << "Specular Color: " << mtl.m_properties.specularColor.x << " " << mtl.m_properties.specularColor.y << " " << mtl.m_properties.specularColor.z << std::endl;
	os << "Specular Exponent: " << mtl.m_properties.specularExponent << std::endl;

	return os;
}