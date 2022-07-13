#pragma once
#include "DirectXCommon.h"
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
#include "ParticleManager.h"
#include "PostEffect.h"
#include "ModelManager.h"
#include <vector>
#include <memory>
#include <list> // ヘッダファイルインクルード
using namespace std;         //  名前空間指定
//前方宣言
class SceneManager;
class CollisionManager;
class Player;
class TouchableObject;
//シーンインターフェース
class BaseScene
{
public:
	//void CreateParticles();
	///ゲームシーン用
	DebugCamera* camera = nullptr;
	Sprite* spriteBG = nullptr;
	Sprite* spritePlayerHP = nullptr;
	Sprite* spriteBossHP = nullptr;
	std::vector<Object3d*> objects;
	Player* player = nullptr;
	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;
	ParticleManager* particleMan = nullptr;
	XMFLOAT3 PlayerPosition = { 1.0f,5.0f,0.0f };
	//当たり判定 レイ
	Ray ray;
	Texture* titleTexture = nullptr;
	//当たり判定
	Collision* collision = nullptr;
	LightGroup* lightGroup = nullptr;
	XMFLOAT3 cameraPos = { 0, 3.0f, -20.0f };
	Enemy* enemy = nullptr;
	//Player* player = nullptr;
	CollisionManager* collsionManager = nullptr;
	PostEffect* postEffect = nullptr;
	Model* modelFloor = nullptr;
	unique_ptr <Object3d> objFloor;
	Model* modelSphere = nullptr;
	unique_ptr <Object3d> objSphere = nullptr;
	Model* modelSkydome = nullptr;
	unique_ptr <Object3d> objSkydome = nullptr;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//点光源
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	//スポットライト
	float spotLightDir[3] = { 0,0,1 };
	float spotLightPos[3] = { 0,0,-2 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f };

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };
	int PostType = 0;

	enum PostType {
		Normal,
		Blur,
		Inversion,
	};
public:
	//仮想デストラクタ
	virtual ~BaseScene() = default;

	//初期化
	virtual void Initiallize(DirectXCommon* dxCommon) = 0;
	//開放
	virtual void Finalize() = 0;
	//更新
	virtual void Update(DirectXCommon* dxCommon) = 0;
	//描画
	virtual void Draw(DirectXCommon* dxCommon) = 0;
};