#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "DebugText.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "WinApp.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "Camera.h"
#include "LightGroup.h"

//フレームワーク
class Framework
{
public:

	//実行
	void Run();
	//初期化
	virtual void Initialize();

	//初期化
	virtual void Finalize();

	//毎フレーム処理
	virtual void Update();

	////描画
	virtual void Draw()  = 0;

	bool GetEndRequst() { return endRequst; }
protected:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	Collision* collision;
	bool endRequst = false;
	WinApp* winApp = nullptr;
	Camera* camera = nullptr;
	AbstractSceneFactory* sceneFactory_ = nullptr;
};