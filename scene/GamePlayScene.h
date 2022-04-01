#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "BaseScene.h"
#include "DebugCamera.h"
#include "LightGroup.h"
#include "FBXObject3d.h"
#include "DirectXCommon.h"
#include "DirectXMath.h"
#include "Texture.h"
#include "Enemy.h"
#include "Player.h"


//ゲームプレイシーン
class GamePlayScene : public BaseScene
{
private://メンバ変数

public:
	//初期化
	void Initiallize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//開放
	void Finalize() override;
	///ゲームシーン用
	DebugCamera* camera = nullptr;
	Sprite* spriteBG = nullptr;
	Sprite* spritePlayerHP = nullptr;
	Sprite* spriteBossHP = nullptr;
	Model* modelGround = nullptr;
	Object3d* objGround;

	Model* modelSkydome = nullptr;
	Object3d* objSkydome;

	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;

	//当たり判定 レイ
	Ray ray;
	Texture* titleTexture = nullptr;
	//当たり判定
	Collision* collision = nullptr;
	LightGroup* lightGroup = nullptr;
	XMFLOAT3 cameraPos = { 0, 3.0f, -20.0f };
	Enemy* enemy = nullptr;
	Player* player = nullptr;
};