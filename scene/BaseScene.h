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
//シーンインターフェース
class BaseScene
{
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