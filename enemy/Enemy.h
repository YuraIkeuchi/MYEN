#pragma once
#include"Object3d.h"
#include"Model.h"
#include <DirectXMath.h>
#include <Input.h>
#include"CollisionPrimitive.h"
#include "Player.h"
class Enemy :
	public Object3d {
public:
	Enemy();

	bool Initialize() override;
	void Update() override;
	void Draw();
	bool CollidePlayer();
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
/// 座標の取得
	const XMFLOAT3& GetPosition() { return  object3d->GetPosition(); }

	const XMFLOAT3& GetRotation() { return object3d->GetRotation(); }
	/// 座標の設定
	void SetPosition(XMFLOAT3 position) { object3d->SetPosition(position); }

	void SetRotation(XMFLOAT3 rotation) { object3d->SetRotation(rotation); }

	void SetPlayer(Player* player) { this->player = player; }

	//bool Collision(XMFLOAT3 position, float radius);
private:
	Object3d* object3d;
	Model* model;
	XMFLOAT3 position = {-10,0,0};
	float rad = 0.4f;
public:
	Sphere collider;
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
	float Fricrion = 0.2f;
	float FricrionPower = 0.0f;
	float weight = 10.0f;
	float MoveFricrion = 0.5f;
	Player* player = nullptr;
	bool hit = false;
};

