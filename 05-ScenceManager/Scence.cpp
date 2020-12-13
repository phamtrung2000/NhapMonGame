#include "Scence.h"

CScene::CScene()
{
}

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}