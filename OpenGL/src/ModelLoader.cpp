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
	//std::cout << std::endl;
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

Model ModelLoader::LoadModelFromOBJFile(const char * modelFilePath)
{
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

				}
				break;

				case Tag::TextureCoordinate:
				{

				}
				break;

				case Tag::VertexNormal:
				{

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
