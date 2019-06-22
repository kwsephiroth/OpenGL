#pragma once
#include "Model.h"
#include <fstream>
#include <iostream>
#include <queue>

enum class Tag
{
	None,
	Vertex,
	TextureCoordinate,
	VertexNormal,
	Face, 
	Smoothed
};

class ModelLoader
{
private:
	static void SplitLineByDelimeter(const std::string& line, char delimiter, std::queue < std::string>& destination);
	static Tag GetTag(const std::string& tagString);

	ModelLoader();
public:
	static Model LoadModelFromOBJFile(const char* modelFilePath);
};