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
const int Max = 3;
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

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelPlayer = nullptr;
	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;
	Object3d* objPlayer = nullptr;
	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;
	
	Texture* titleTexture = nullptr;
	Texture* fantasyTexture = nullptr;
	Texture* enemyTexture[Max] = { nullptr };

	XMFLOAT3 PlayerPosition = { -5.0f,0.0f,0.0f };
	XMFLOAT3 FighterPosition = { 5.0f,0.0f,0.0f };
	XMFLOAT3 FloorPosition = { 0.0f,-2.0f,0.0f };
	XMFLOAT3 texpo = { 0,0,0 };
	XMFLOAT3 fantasypos = { 0,0,0 };
	XMFLOAT3 enepos[Max];
	
	//当たり判定 球
	Sphere sphere;
	//当たり判定 平面
	Plane plane;
	//当たり判定 三角形
	Triangle triangle;
	//当たり判定 レイ
	Ray ray;
	//当たり判定
	Collision* collision = nullptr;
	LightGroup* lightGroup = nullptr;
	XMFLOAT3 cameraPos = { 0, 3.0f, -20.0f };
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	float PI = 3.14f;
	float Playerradius = 0.0f;
	float PlayerSpeed = 0.0f;
	float Playerscale = 20.0f;// LaneNumと一緒に変えること
	float PlayerCircleX = 0.0f;
	float PlayerCircleZ = 0.0f;

	float fantasyradius = 0.0f;
	float fantasySpeed = 0.0f;
	float fantasyscale = 20.0f;// LaneNumと一緒に変えること
	float fantasyCircleX = 0.0f;
	float fantasyCircleZ = 0.0f;
	bool fantasyFlag = false;
	//敵関係
	float Enemyradius[Max] = { 0.0f };
	float EnemySpeed[Max] = { 0.0f };
	float Enemyscale[Max] = { 0.0f };// LaneNumと一緒に変えること
	float EnemyCircleX[Max] = { 0.0f };
	float EnemyCircleZ[Max] = { 0.0f };
};