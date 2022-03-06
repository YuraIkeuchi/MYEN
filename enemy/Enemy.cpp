#include "Enemy.h"
#include "Input.h"
#include"Collision.h"
#include<sstream>
#include<iomanip>
using namespace DirectX;

Enemy::Enemy() {
	model = Model::LoadFromOBJ("Block");
	object3d = new Object3d();
}

void Enemy::Initialize() {
	object3d = Object3d::Create();
	object3d->SetModel(model);
	object3d->SetPosition(pos);
	object3d->SetScale({ 0.5,0.5,0.5 });
	collider.radius=rad;
}

void Enemy::Update() {
	XMFLOAT3 pos = this->object3d->GetPosition();
	object3d->Update();
	collider.center = XMVectorSet(pos.x,pos.y,pos.z,1);
}

void Enemy::Draw() {
	Object3d::PreDraw();
	object3d->Draw();
}

bool Enemy::Collision(XMFLOAT3 position,float radius) {
	XMFLOAT3 pos=this->object3d->GetPosition();
	float d = pow(pos.x - position.x, 2)
		+ pow(pos.y - position.y, 2)
		+ pow(pos.z - position.z, 2);
	float r1r2 = pow(rad +radius, 2);
	if (d < r1r2) {
		return true;
	} else {
		return false;
	}
}
