#include "../ModelLoader.h"

void ModelLoader::SplitLineByDelimeter(const std::string & line, char delimiter, std::queue<std::string>& segments)
{
	std::string segment;
	int j = 0;
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (line[i] == delimiter)
		{
			++j;
			//std::cout << "Segment [" << j << "] : " << segment << std::endl;
			segments.push(segment);
			segment.clear();
			while (line[i] == delimiter)//Move passed consecutive occurrences of delimiter
				++i;
		}
		if(i < line.size())
			segment += line[i];
	}

	segments.push(segment);
	//std::cout << "Segment [" << j << "] : " << segment << std::endl << std::endl;
}

Tag ModelLoader::GetTag(const std::string & tagString)
{
	if (tagString == "v")
		return Tag::Vertex;
	else if (tagString == "vt")
		return Tag::TextureCoordinate;
	else if (tagString == "vn")
		return Tag::VertexNormal;
	else if (tagString == "f")
		return Tag::Face;
	else if (tagString == "s")
		return Tag::Smoothed;
	else
		return Tag::None;
}

void ModelLoader::StoreVertex(std::queue<std::string>& vertexStrings, Model & destinationModel)
{
	if (vertexStrings.empty())
		return;

	if (vertexStrings.size() != 3)
	{
		std::cout << "ERROR: Failed to store model vertex : Exactly three components expected." << std::endl;
		return;
	}

	try
	{
		auto x = std::stof(vertexStrings.front());
		vertexStrings.pop();
		
		auto y = std::stof(vertexStrings.front());
		vertexStrings.pop();

		auto z = std::stof(vertexStrings.front());
		vertexStrings.pop();

		destinationModel.m_vertices.emplace_back( x, y, z );
		
		//TODO: write ostream overload for easy debugging
		//std::cout << "VERTICES:" << std::endl;
		//const auto& vertex = destinationModel.m_vertices.back();
		//std::cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: Failed to store model vertex : " << e.what() << std::endl;
	}
}

void ModelLoader::StoreTextureCoordinate(std::queue<std::string>& textureCoordinateStrings, Model & destinationModel)
{
	if (textureCoordinateStrings.empty())
		return;

	if (textureCoordinateStrings.size() != 2)
	{
		std::cout << "ERROR: Failed to store model texture coordinate: Exactly two components expected." << std::endl;
		return;
	}

	try
	{
		auto s = std::stof(textureCoordinateStrings.front());
		textureCoordinateStrings.pop();

		auto t = std::stof(textureCoordinateStrings.front());
		textureCoordinateStrings.pop();

		destinationModel.m_textureCoordinates.emplace_back(s, t);

		//TODO: write ostream overload for easy debugging
		//std::cout << "TEXTURE COORDINATES:" << std::endl;
		//const auto& vertex = destinationModel.m_textureCoordinates.back();
		//std::cout << "(" << vertex.s << ", " << vertex.t << ")" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: Failed to store model texture coordinate: " << e.what() << std::endl;
	}
}

void ModelLoader::StoreVertexNormal(std::queue<std::string>& vertexNormalStrings, Model & destinationModel)
{
	if (vertexNormalStrings.empty())
		return;

	if (vertexNormalStrings.size() != 3)
	{
		std::cout << "ERROR: Failed to store model vertex normal: Exactly three components expected." << std::endl;
		return;
	}

	try
	{
		auto x = std::stof(vertexNormalStrings.front());
		vertexNormalStrings.pop();

		auto y = std::stof(vertexNormalStrings.front());
		vertexNormalStrings.pop();

		auto z = std::stof(vertexNormalStrings.front());
		vertexNormalStrings.pop();

		destinationModel.m_normals.emplace_back(x, y, z);

		//TODO: write ostream overload for easy debugging
		//std::cout << "NORMALS:" << std::endl;
		//const auto& vertex = destinationModel.m_normals.back();
		//std::cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: Failed to store model vertex normal: " << e.what() << std::endl;
	}
}

Model ModelLoader::LoadModelFromOBJFile(const char * modelFilePath)
{
	std::cout << "Loading model at path '" << modelFilePath << "' ..." << std::endl;
	Model loadedModel;
	std::ifstream modelFileStream(modelFilePath, std::ios::in);

	if (!modelFileStream.good())
	{
		std::cout << "ERROR: Failed to open file at path '" << modelFilePath << "'" << std::endl;
		loadedModel.m_initialized = false;
	}
	else
	{
		int i = 1;
		std::string line;
		while (!modelFileStream.eof())
		{
			std::getline(modelFileStream, line);
			std::queue < std::string > lineSegments;
			SplitLineByDelimeter(line, ' ', lineSegments);

			const std::string lineTagString = lineSegments.front();
			lineSegments.pop();//Remove front element from queue
			auto lineTag = GetTag(lineTagString);

			switch (lineTag)
			{
				case Tag::Vertex:
				{
					//std::cout << "Component Count = " << lineSegments.size() << std::endl;
					StoreVertex(lineSegments, loadedModel);
				}
				break;

				case Tag::TextureCoordinate:
				{
					//std::cout << "Component Count = " << lineSegments.size() << std::endl;
					StoreTextureCoordinate(lineSegments, loadedModel);
				}
				break;

				case Tag::VertexNormal:
				{
					//std::cout << "Component Count = " << lineSegments.size() << std::endl;
					StoreVertexNormal(lineSegments, loadedModel);
				}
				break;

				case Tag::Face:
				{

				}
				break;

				case Tag::Smoothed:
				{

				}
				break;

				default:
				{
					//std::cout << "Ignoring the following line:\n" << line << std::endl;
					//std::cout << "lineTag = " << (unsigned short)lineTag << std::endl;
					//std::cout << "lineTagString = \"" << lineTagString << "\"" << std::endl << std::endl;
				}
				break;
			}

			++i;
		}
	}

	return loadedModel;
}
