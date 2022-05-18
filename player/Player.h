#pragma once
#include "Object3d.h"
#include"Model.h"
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
	void Draw();

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
	const XMFLOAT3& GetPosition() { return  object3d->GetPosition(); }

	const XMFLOAT3& GetRotation() { return object3d->GetRotation(); }

	const bool& GetOnGround() { return onGround; }

	const float& Getvx() { return vx; }

	void SetPosition(XMFLOAT3 position) { object3d->SetPosition(position); }

	void SetRotation(XMFLOAT3 rotation) { object3d->SetRotation(rotation); }

	void Setvx(const float vx) { this->vx = vx; }

private:
	XMFLOAT3 pos = { 0,40,0 };
	Object3d* object3d;
	Model* model;
	int hit = 0;
	bool onGround = true;
	// 落下ベクトル
	DirectX::XMVECTOR fallV;
	//重力加速度
	const float GravityY = -1.2f / 60.0f;
	const float GravityX = -0.4f / 60.0f;
	//速さ
	float vy = 0.0f;
	float vx = 0.0f;
	//初速度
	float FirstSpeedY = 0.0f;
	float FirstSpeedX = 0.0f;
	//ImGui用
	float AddSpeed = 0.0f;
	//大砲フラグ
	bool ShotFlag = false;
	int imageTimer = 0;
	//摩擦関係
	float Fricrion = 0.5f;
	float FricrionPower = 0.0f;
	float weight = 1000.0f;
	float MoveFricrion = 0.5f;
};

