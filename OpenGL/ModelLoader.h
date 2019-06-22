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

	static void StoreVertex(std::queue<std::string>& vertexStrings, Model& destinationModel);
	static void StoreTextureCoordinate(std::queue<std::string>& textureCoordinateStrings, Model& destinationModel);
	static void StoreVertexNormal(std::queue<std::string>& vertexNormalStrings, Model& destinationModel);

	ModelLoader();
public:
	static Model LoadModelFromOBJFile(const char* modelFilePath);
};