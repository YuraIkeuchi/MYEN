#pragma once
#include"Object3d.h"
#include"Model.h"
#include <DirectXMath.h>
#include <Input.h>
#include"CollisionPrimitive.h"
class Player : public Object3d{
public:
	Player();

	bool Initialize()override;
	void Update()override;
	void Draw();
private:
	// DirectX::‚ðÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// À•W‚ÌŽæ“¾
	const XMFLOAT3& GetPosition() { return  object3d->GetPosition(); }

	const XMFLOAT3& GetRotation() { return object3d->GetRotation(); }
	/// À•W‚ÌÝ’è
	void SetPosition(XMFLOAT3 position) { object3d->SetPosition(position); }

	void SetRotation(XMFLOAT3 rotation) { object3d->SetRotation(rotation); }

	void OnCollision(const CollisionInfo& info)override;
private:
	Object3d* object3d;
	Model* model;
	XMFLOAT3 pos = { 10,0,0 };
	float rad = 0.4f;
public:
	Sphere collider;
};

