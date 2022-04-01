#include "Player.h"
#include "Input.h"
#include"Collision.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include<sstream>
#include<iomanip>
using namespace DirectX;

Player::Player() {
	model = Model::LoadFromOBJ("chr_sword");
	object3d = new Object3d();
}

bool Player::Initialize() {
	if (!Object3d::Initialize())
	{
		return false;
	}

	object3d = Object3d::Create();
	object3d->SetModel(model);
	object3d->SetPosition(pos);
	object3d->SetScale({ 1.5,1.5,1.5 });
	collider.radius = rad;
	//コライダーの追加
	float radius = 0.6;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	return true;
}

void Player::Update() {
	Input* input = Input::GetInstance();
	if (input->LeftTiltStick(input->Right)) {
			pos.x += 0.2;
	}

	if (input->LeftTiltStick(input->Left)) {
			pos.x -= 0.2;
	}

	if (input->LeftTiltStick(input->Up)) {
			pos.z += 0.2;
	}

	if (input->LeftTiltStick(input->Down)) {
		pos.z -= 0.2;
	}
	object3d->Update();
	collider.center = XMVectorSet(pos.x, pos.y, pos.z, 1);
	object3d->SetPosition(pos);
}

void Player::Draw() {
	Object3d::PreDraw();
	object3d->Draw();
}

void Player::OnCollision(const CollisionInfo& info)
{
	DebugText::GetInstance()->Print("Collision detected.",900,50,50);


	for (int i = 0; i < 1; ++i) {

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		ParticleManager::GetInstance()->Add(10, XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
	}
}

