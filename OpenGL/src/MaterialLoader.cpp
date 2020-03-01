#include "../MaterialLoader.h"



void MaterialLoader::SplitLineByDelimeter(const std::string& line, char delimiter, std::queue<std::string>& destination)
{
	std::string segment;
	int j = 0;
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (line[i] == delimiter)
		{
			++j;
			//std::cout << "Segment [" << j << "] : " << segment << std::endl;
			destination.push(segment);
			segment.clear();

			++i;

			while (line[i] == delimiter)//Move passed consecutive occurrences of delimiter
				++i;
		}
		if (i < line.size())
			segment += line[i];
	}

	destination.push(segment);
	//std::cout << "Segment [" << j << "] : " << segment << std::endl << std::endl;
}

std::unique_ptr<MaterialLoader::MaterialsMap> MaterialLoader::LoadMaterialFromMtlFile(const std::string& mtlFilePath)
{
	std::cout << "MaterialLoader::LoadMaterialFromMtlFile - Loading materials from mtl file at path '" << mtlFilePath << "' ..." << std::endl;

	if (mtlFilePath.empty())
	{
		std::cout << "ERROR: Empty obj file path. Please provide a valid obj file path." << std::endl;
		return nullptr;
	}

	auto mtlMapPtr = std::make_unique<MaterialLoader::MaterialsMap>();

	std::ifstream mtlFileStream(mtlFilePath, std::ios::in);
	int numFaces = 0;

	if (!mtlFileStream.good())
	{
		std::cout << "ERROR: Failed to open file at path '" << mtlFilePath << "'" << std::endl;
	}
	else
	{
		std::string line;
		Material* currentMtlPtr = nullptr;

		while (!mtlFileStream.eof())
		{
			std::getline(mtlFileStream, line);

			if (line.empty())
				continue;

			std::queue < std::string > lineSegments;
			SplitLineByDelimeter(line, ' ', lineSegments);
			
			const auto lineBeginStr = std::move(lineSegments.front());
			lineSegments.pop();

			if (lineBeginStr == "newmtl")
			{
				//Start of new material
				auto mtlName = std::move(lineSegments.front());
				mtlMapPtr->emplace(mtlName, Material(mtlName));
				currentMtlPtr = &mtlMapPtr->find(mtlName)->second;
				lineSegments.pop();
			}
			else if (lineBeginStr == "Ns")/* || lineBeginStr == "Ni" || lineBeginStr == "d")*/
			{
				try 
				{
					if (currentMtlPtr)
					{
						currentMtlPtr->m_properties.specularExponent = std::stod(std::move(lineSegments.front()));
						lineSegments.pop();
					}
				}
				catch (std::exception e)
				{
					//std::cout << "ERROR: Failed to : " << e.what() << std::endl;
					return nullptr;
				}
			}
			else if (lineBeginStr == "Ka")
			{
				try
				{
					if (currentMtlPtr)
					{
						if (lineSegments.size() != 3)
							continue;

						float R = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();
						
						float G = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();
						
						float B = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						currentMtlPtr->m_properties.ambientColor = { R, G, B };
					}
				}
				catch (std::exception e)
				{
					//std::cout << "ERROR: Failed to : " << e.what() << std::endl;
					return nullptr;
				}
			}
			else if (lineBeginStr == "Kd")
			{
				try
				{
					if (currentMtlPtr)
					{
						if (lineSegments.size() != 3)
							continue;

						float R = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						float G = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						float B = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						currentMtlPtr->m_properties.diffuseColor = { R, G, B };
					}
				}
				catch (std::exception e)
				{
					//std::cout << "ERROR: Failed to : " << e.what() << std::endl;
					return nullptr;
				}
			}
			else if (lineBeginStr == "Ks")
			{
				try
				{
					if (currentMtlPtr)
					{
						if (lineSegments.size() != 3)
							continue;

						float R = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						float G = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						float B = std::stof(std::move(lineSegments.front()));
						lineSegments.pop();

						currentMtlPtr->m_properties.specularColor = { R, G, B };
					}
				}
				catch (std::exception e)
				{
					//std::cout << "ERROR: Failed to : " << e.what() << std::endl;
					return nullptr;
				}
			}
			//else if (lineBeginStr == "illum")
			//{

			//}
		}
	}

	for (const auto& pair : *mtlMapPtr)
	{
		std::cout << pair.second << std::endl;
	}

	return mtlMapPtr;
}