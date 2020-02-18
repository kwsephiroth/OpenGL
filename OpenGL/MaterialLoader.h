#pragma once
#include "Material.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <queue>

class MaterialLoader
{
private:
	MaterialLoader() = default;
	static void SplitLineByDelimeter(const std::string& line, char delimiter, std::queue < std::string>& destination);
	
public:
	~MaterialLoader() = default;

	using MaterialsMap = std::unordered_map<std::string, Material>;

	static std::unique_ptr<MaterialsMap> LoadMaterialFromMtlFile(const std::string& mtlFilePath);
};

