#pragma once
#include "Model.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <memory>

enum class Tag
{
	None,
	ObjectName,
	Vertex,
	TextureCoordinate,
	VertexNormal,
	Face, 
	Smoothed
};

enum class ErrorCode
{
	Fail=-1,
	Success=1
};

class ModelLoader
{
private:
	static void SplitLineByDelimeter(const std::string& line, char delimiter, std::queue < std::string>& destination);
	static void SplitFaceLine(const std::string& line, std::queue < std::string>& destination);
	static Tag GetTag(const std::string& tagString);

	static void StoreVertexLocation(std::queue<std::string>& vertexSegments, Model& destinationModel);
	static void StoreTextureCoordinate(std::queue<std::string>& textureCoordinateSegments, Model& destinationModel);
	static void StoreVertexNormal(std::queue<std::string>& vertexNormalSegments, Model& destinationModel);
	static void StoreFace(std::queue<std::string>& faceSegments, Model& destinationModel);

	static void LoadTexture(const char * textureFilePath, Model& destinationModel);

	ModelLoader();
public:
	static Model LoadModelFromOBJFile(const char* modelFilePath, const char* textureFilePath = nullptr);
};