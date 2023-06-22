#pragma once
#include "Model.h"
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <memory>
#include <SOIL2/soil2.h>

enum class Tag
{
	None,
	ObjectName,
	Vertex,
	TextureCoordinate,
	VertexNormal,
	Face, 
	Smoothed,
	Material
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
	static void StoreFace(std::queue<std::string>& faceSegments, Model& destinationModel, const std::string& currentMtlName);
	static void LoadTexture(const char * textureFilePath, Model& destinationModel);

	ModelLoader();
public:
	static std::unique_ptr<Model> LoadModelFromOBJFile(const std::string& modelName, const std::string& modelFilePath, const std::string& textureFilePath,  const glm::vec3 initalWorldPosition = { 0,0,0 });
};