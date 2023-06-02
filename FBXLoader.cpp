#include "FbxLoader.h"

void FbxLoader::Initialize()
{
}

void FbxLoader::Finalize()
{
}

void FbxLoader::LoadModel(fbxModel* model, const std::string& modelDirectory, bool inverseU_, bool inverseV_, bool animeFlag)
{
}


FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}