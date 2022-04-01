#include "Player.h"
#include "Input.h"
#include"Collision.h"
#include<sstream>
#include<iomanip>
using namespace DirectX;

Player::Player() {
	model = Model::LoadFromOBJ("chr_sword");
	object3d = new Object3d();
}

void Player::Initialize() {
	object3d = Object3d::Create();
	object3d->SetModel(model);
	object3d->SetPosition(pos);
	object3d->SetScale({ 1.5,1.5,1.5 });
	collider.radius = rad;
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

