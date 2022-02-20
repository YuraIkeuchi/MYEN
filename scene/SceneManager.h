#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <string>

//シーン管理
class SceneManager
{
public:
	static SceneManager* GetInstance();
	//更新
	void Update();
	/// 描画
	void Draw();

public:
	//次シーン予約
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	void operator =(const SceneManager&) = delete;
};