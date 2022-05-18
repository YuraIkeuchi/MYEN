#include "Enemy.h"
#include "Input.h"
#include"Collision.h"
#include<sstream>
#include<iomanip>
using namespace DirectX;

Enemy::Enemy() {
	model = Model::LoadFromOBJ("sphere");
	object3d = new Object3d();
}

bool Enemy::Initialize() {
	object3d = Object3d::Create();
	object3d->SetModel(model);
	position = { 150.0f,0.0f,0.0f };
	object3d->SetPosition(position);
	object3d->SetScale({ 1.7f,1.7f,1.7f });

	//// �R���C�_�[�̒ǉ�
	//float radius = 0.6f;
	//SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	//collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Enemy::Update() {
	Input* input = Input::GetInstance();
	//�v���C���[�ɏd�͂�����
	if (input->TriggerButton(input->Button_A) || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		FirstSpeedY = AddSpeed;
		FirstSpeedX = 2.0f;
	}

	if (input->TriggerKey(DIK_A)) {
		//a -= 0.02;
	}
	//�����鏈��
	//if (onGround == false) {
	//	vy += GravityY;
	//	position.y += vy + FirstSpeedY;
	//	//��C�̏ꍇ
	//	if (ShotFlag == true) {
	//		if (vx >= -1.8f) {
	//			vx += GravityX;
	//		}
	//		position.x += vx + FirstSpeedX;
	//	}
	//}

	////���Ƃɖ߂�
	//if (position.y <= -200.0f) {
	//	onGround = true;
	//	vy = 0.0f;
	//	vx = 0.0f;
	//	position = { -150.0f,0.0f,0.0f };
	//	AddSpeed = 0.0f;
	//}

	//���C�֌W
	if (onGround == false) {

		float g = weight * 9.8f;
		MoveFricrion -= 0.01f;
		FricrionPower = g * Fricrion;
		vx = FirstSpeedX + FricrionPower / weight * MoveFricrion;
		position.x -= vx;
		//if (vx >= 0.0f) {
		//
		//}
		//else {
		//	onGround = true;
		//	vy = 0.0f;
		//	vx = 0.0f;
		//	MoveFricrion = 0.5f;
		//	//position = { -150.0f,0.0f,0.0f };
		//}

		//��C�̏ꍇ
	/*	if (ShotFlag == true) {
			if (vx >= -1.8f) {
				vx += GravityX;
			}
			position.x += vx + FirstSpeedX;
		}*/

		//if (vx <= 0.0f) {
		//	vx = 0.0f;
		//	MoveFricrion = 0.0f;
		//}
	/*	if (position.x >= 300) {
			position.x = -150.0f;
		}*/
	}

	//���Ƃɖ߂�
	if (position.y <= -200.0f) {
		onGround = true;
		vy = 0.0f;
		vx = 0.0f;
		position = { 150.0f,0.0f,0.0f };
		AddSpeed = 0.0f;
	}

	//�f�o�b�O�e�L�X�g
	//if (onGround == true) {
	//	DebugText::GetInstance()->Print("PUSH SPACE", 900, 600, 1);
	//}
	//DebugText::GetInstance()->Print("PlayerSetting by ImGui", 900, 650, 1);
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	CollidePlayer();
	object3d->SetPosition(position);
	// �s��̍X�V�Ȃ�
	object3d->Update();
}

void Enemy::Draw() {
	ImGui::Begin("test");
	if (ImGui::TreeNode("Debug")) {
		if (ImGui::TreeNode("Enemy")) {
			ImGui::SliderFloat("FricrionPower", &FricrionPower, 25, -25);
			ImGui::SliderFloat("vx", &vx, 25, -25);
			ImGui::SliderFloat("MoveFriction", &MoveFricrion, 25, -25);
			ImGui::Unindent();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
	Object3d::PreDraw();
	object3d->Draw();
}

//�v���C���[���_���[�W��H�炤
bool Enemy::CollidePlayer() {

	XMFLOAT3 playerpos = player->GetPosition();
	float plavx = player->Getvx();
	if (Collision::SphereCollision(position.x, position.y, position.z, 0.5f, playerpos.x, playerpos.y, playerpos.z, 0.5) == true) {
		//player->Setvx(vx * -1);
		vx = plavx * -1;
		return true;
	}
	else {
		return false;
	}
}

//bool Enemy::Collision(XMFLOAT3 position,float radius) {
//	XMFLOAT3 pos=this->object3d->GetPosition();
//	float d = pow(pos.x - position.x, 2)
//		+ pow(pos.y - position.y, 2)
//		+ pow(pos.z - position.z, 2);
//	float r1r2 = pow(rad +radius, 2);
//	if (d < r1r2) {
//		return true;
//	} else {
//		return false;
//	}
//}
