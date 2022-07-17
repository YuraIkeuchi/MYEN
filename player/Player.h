#pragma once
#include "Object3d.h"
#include"Model.h"
#include <memory>
#include <list> // ヘッダファイルインクルード
using namespace std;         //  名前空間指定
class Player :
	public Object3d
{
public:
	Player();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	//static Player* Create(Model* model = nullptr);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(int DrawNumber);

	/// <summary>
	/// 開放
	/// </summary>
	void Finalize();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;
public:
	const XMFLOAT3& GetPosition() { return  pos; }

	const XMFLOAT3& GetRotation() { return object3d->GetRotation(); }

	void SetPosition(XMFLOAT3 position) { object3d->SetPosition(position); }

	void SetRotation(XMFLOAT3 rotation) { object3d->SetRotation(rotation); }

private:
	XMFLOAT3 pos = { 0,-2,0 };
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	unique_ptr <Object3d> object3d;
	unique_ptr <Object3d> Toon_object3d;
	unique_ptr <Object3d> Single_object3d;
	Model* model;
	int hit = 0;
	bool onGround = true;
	// 落下ベクトル
	DirectX::XMVECTOR fallV;
	enum Shader {
		normal,
		one,
	};

	int Shadernumber = 0;
};

