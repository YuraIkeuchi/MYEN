#include"ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model;
std::map<ModelManager::FBXName, FBXModel*>  ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance() {
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize() {
	/*LoadModel(Player, "cha_knight");
	LoadModel(Enemy, "chr_sword");*/
	LoadModel(Player, "UVSphere");
	LoadFBXModel(Pla, "Motti_move");
	LoadFBXModel(Motti_moveF, "Motti_moveF");
	LoadFBXModel(Test, "Knight");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName) {
	model[modelName] = new Model();
	model[modelName] = Model::LoadFromOBJ(fileName);
}
void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName) {
	fbxModel[modelName] = new FBXModel();
	fbxModel[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
}

