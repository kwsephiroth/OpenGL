#include "../ModelLoader.h"

void ModelLoader::SplitLineByDelimeter(const std::string & line, char delimiter, std::queue<std::string>& destination)
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
		if(i < line.size())
			segment += line[i];
	}

	destination.push(segment);
	//std::cout << "Segment [" << j << "] : " << segment << std::endl << std::endl;
}

void ModelLoader::SplitFaceLine(const std::string & line, std::queue<std::string>& destination)
{
	std::string segment;
	int j = 0;
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (line[i] == '/')
		{
			//++j;
			//std::cout << "Segment [" << j << "] : " << segment << std::endl;
			destination.push(segment);
			segment.clear();

			++i;

			while (line[i] == '/')//Move passed consecutive occurrences of delimiter
			{
				destination.push("");//Push an empty string
				++i;
			}
		}
		if (i < line.size())
			segment += line[i];
	}

	destination.push(segment);
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

void ModelLoader::StoreVertexLocation(std::queue<std::string>& vertexSegments, Model & destinationModel)
{
	if (vertexSegments.empty())
	{
		destinationModel.m_initialized = false;
		return;
	}

	if (vertexSegments.size() != 3)
	{
		std::cout << "ERROR: Failed to store model vertex : Exactly three components expected." << std::endl;
		return;
	}

	//try
	//{
		auto x = std::stof(vertexSegments.front());
		vertexSegments.pop();
		
		auto y = std::stof(vertexSegments.front());
		vertexSegments.pop();

		auto z = std::stof(vertexSegments.front());
		vertexSegments.pop();

		destinationModel.m_positions.emplace_back( x, y, z );
		
		//TODO: write ostream overload for easy debugging
		//const auto& vertex = destinationModel.m_positions.back();
		//std::cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
	//}
	//catch (std::exception e)
	//{
	//	std::cout << "ERROR: Failed to store model vertex : " << e.what() << std::endl;
	//	destinationModel.m_initialized = false;
	//}
}

void ModelLoader::StoreTextureCoordinate(std::queue<std::string>& textureCoordinateSegments, Model & destinationModel)
{
	if (textureCoordinateSegments.empty())
	{
		destinationModel.m_initialized = false;
		return;
	}

	if (textureCoordinateSegments.size() != 2)
	{
		std::cout << "ERROR: Failed to store model texture coordinate: Exactly two components expected." << std::endl;
		destinationModel.m_initialized = false;
		return;
	}

	//try
	//{
		auto s = std::stof(textureCoordinateSegments.front());
		textureCoordinateSegments.pop();

		auto t = std::stof(textureCoordinateSegments.front());
		textureCoordinateSegments.pop();

		destinationModel.m_textureCoordinates.emplace_back(s, t);

		//TODO: write ostream overload for easy debugging
		//const auto& vertex = destinationModel.m_textureCoordinates.back();
		//std::cout << "(" << vertex.s << ", " << vertex.t << ")" << std::endl;
	//}
	//catch (std::exception e)
	//{
	//	std::cout << "ERROR: Failed to store model texture coordinate: " << e.what() << std::endl;
	//	destinationModel.m_initialized = false;
	//}
}

void ModelLoader::StoreVertexNormal(std::queue<std::string>& vertexNormalSegments, Model & destinationModel)
{
	if (vertexNormalSegments.empty())
	{
		destinationModel.m_initialized = false;
		return;
	}

	if (vertexNormalSegments.size() != 3)
	{
		std::cout << "ERROR: Failed to store model vertex normal: Exactly three components expected." << std::endl;
		destinationModel.m_initialized = false;
		return;
	}

	//try
	//{
		auto x = std::stof(vertexNormalSegments.front());
		vertexNormalSegments.pop();

		auto y = std::stof(vertexNormalSegments.front());
		vertexNormalSegments.pop();

		auto z = std::stof(vertexNormalSegments.front());
		vertexNormalSegments.pop();

		destinationModel.m_normals.emplace_back(x, y, z);

		//TODO: write ostream overload for easy debugging

		//const auto& vertex = destinationModel.m_normals.back();
		//std::cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
	//}
	//catch (std::exception e)
	//{
	//	std::cout << "ERROR: Failed to store model vertex normal: " << e.what() << std::endl;
	//	destinationModel.m_initialized = false;
	//}
}

void ModelLoader::StoreFace(std::queue<std::string>& faceSegments, Model & destinationModel)
{
	if (faceSegments.empty())
		return;

	if (faceSegments.size() != 3)
	{
		std::cout << "ERROR: Failed to store model face: Exactly three components expected." << std::endl;
		destinationModel.m_initialized = false;
		return;
	}

	while (!faceSegments.empty())
	{
		int positionIndex, textureCoorIndex, normalIndex;

		//example 2/7/3 5/8/3 3/9/3
		std::queue<std::string> indexSegments;
		SplitFaceLine(faceSegments.front(), indexSegments);
		
		auto indicesCount = indexSegments.size();
		//std::cout << indicesCount << " ";

		if (indicesCount >= 1)
		{
			auto& indexStr = indexSegments.front(); //std::cout << indexStr << "/";
			
			positionIndex = (indexStr.empty() ? -1 : (std::stoi(indexStr) - 1));
			indexSegments.pop();

			if (indicesCount > 1)
			{
				indexStr = indexSegments.front(); //std::cout << indexStr << "/";
				textureCoorIndex = (indexStr.empty() ? -1 : (std::stoi(indexStr) - 1));
				//std::cout << textureCoorIndex << std::endl;
				indexSegments.pop();
			}
			
			if (indicesCount > 2)
			{
				indexStr = indexSegments.front(); //std::cout << indexStr << " ";
				normalIndex = (indexStr.empty() ? -1 : (std::stoi(indexStr) - 1));
				indexSegments.pop();
			}

			//Now finally add a position, texture coordinate, and normal
			Vertex v;
			if (positionIndex > -1 && positionIndex < (int)(destinationModel.m_positions.size()))
			{
				const auto & position = destinationModel.m_positions[positionIndex];
				v.position[0] = position.x;
				v.position[1] = position.y;
				v.position[2] = position.z;
			}
			else
			{
				std::cout << "ERROR: Invalid vertex position index '" << positionIndex << "'" << std::endl;
				v.position[0] = 0;
				v.position[1] = 0;
				v.position[2] = 0;
			}

			if (textureCoorIndex > -1 && textureCoorIndex < (int)(destinationModel.m_textureCoordinates.size()))
			{
				const auto & textCoordinate = destinationModel.m_textureCoordinates[textureCoorIndex];
				v.textureCoordinates[0] = textCoordinate.s;
				v.textureCoordinates[1] = textCoordinate.t;
			}
			else
			{
				std::cout << "ERROR: Invalid texture coordinate index '" << textureCoorIndex << "'" << std::endl;
				v.textureCoordinates[0] = 0;
				v.textureCoordinates[1] = 0;
			}

			if (normalIndex > -1 && normalIndex < (int)(destinationModel.m_normals.size()))
			{
				const auto & normal = destinationModel.m_normals[normalIndex];
				v.normal[0] = normal.x;
				v.normal[1] = normal.y;
				v.normal[2] = normal.z;
			}
			else
			{
				std::cout << "ERROR: Invalid normal index '" << normalIndex << "'" << std::endl;
				v.normal[0] = 0;
				v.normal[1] = 0;
				v.normal[2] = 0;
			}

		    std::cout << "(" << v.textureCoordinates[0] << ", " << v.textureCoordinates[1] << ")" << std::endl;
			destinationModel.m_vertices.push_back(std::move(v));
		}

		faceSegments.pop();
	}

	//std::cout << std::endl;
}

Model ModelLoader::LoadModelFromOBJFile(const char * modelFilePath)
{
	std::cout << "ModelLoader::LoadModelFromOBJFile - Loading model at path '" << modelFilePath << "' ..." << std::endl;
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

			//std::cout << "Component Count = " << lineSegments.size() << std::endl;

			switch (lineTag)
			{
				case Tag::Vertex:
				{
					try
					{
						StoreVertexLocation(lineSegments, loadedModel);
					}
					catch (std::exception e)
					{
						std::cout << "ERROR: Failed to store model vertex: " << e.what() << std::endl;
						loadedModel.m_initialized = false;
						return loadedModel;
					}
				}
				break;

				case Tag::TextureCoordinate:
				{
					try
					{
						StoreTextureCoordinate(lineSegments, loadedModel);
					}
					catch (std::exception e)
					{
						std::cout << "ERROR: Failed to store model texture coordinate: " << e.what() << std::endl;
						loadedModel.m_initialized = false;
						return loadedModel;
					}
				}
				break;

				case Tag::VertexNormal:
				{
					try
					{
						StoreVertexNormal(lineSegments, loadedModel);
					}
					catch (std::exception e)
					{
						std::cout << "ERROR: Failed to store model vertex normal: " << e.what() << std::endl;
						loadedModel.m_initialized = false;
						return loadedModel;
					}
				}
				break;

				case Tag::Face:
				{
					try
					{
						StoreFace(lineSegments, loadedModel);
					}
					catch (std::exception e)
					{
						std::cout << "ERROR: Failed to store model face: " << e.what() << std::endl;
						loadedModel.m_initialized = false;
						return loadedModel;
					}
				}
				break;

				case Tag::Smoothed:
				{
					if (lineSegments.front() == "off")
						loadedModel.m_smoothed = false;
					else
						loadedModel.m_smoothed = true;

					lineSegments.pop();
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
	
	std::cout << "Number of positions: " << loadedModel.m_positions.size() << std::endl;
	std::cout << "Number of texture coordinates: " << loadedModel.m_textureCoordinates.size() << std::endl;
	std::cout << "Number of normals: " << loadedModel.m_normals.size() << std::endl;
	std::cout << "ModelLoader::LoadModelFromOBJFile - Model loaded complete." << std::endl;

	return loadedModel;
}
