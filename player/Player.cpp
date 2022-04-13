#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"


using namespace DirectX;

Player::Player() {
	model = Model::LoadFromOBJ("sphere");
	object3d = new Object3d();
}

bool Player::Initialize()
{
	object3d = Object3d::Create();
	object3d->SetModel(model);
	position = { -150.0f,0.0f,0.0f };
	object3d->SetPosition(position);
	object3d->SetScale({ 1.7f,1.7f,1.7f });

	// コライダーの追加
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	object3d->Update();
	// A,Dで旋回
	if (input->PushKey(DIK_A)) {
		rotation.y -= 2.0f;
	} else if (input->PushKey(DIK_D)) {
		rotation.y += 2.0f;
	}

	// 移動ベクトルをY軸周りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	if (input->LeftTiltStick(input->Right)) {
		position.x += 0.1f;
	}
	if (input->LeftTiltStick(input->Left)) {
		position.x -= 0.1f;
	}

	if (input->LeftTiltStick(input->Up)) {
		position.y += 0.1f;
	}

	if (input->LeftTiltStick(input->Down)) {
		position.x -= 0.1f;
	}

	//プレイヤーに重力かかる
	if (input->TriggerButton(input->Button_A) || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		FirstSpeedY = AddSpeed;
		FirstSpeedX = 2.0f;
	}

	//落ちる処理
	if (onGround == false) {
		vy += GravityY;
		position.y += vy + FirstSpeedY;
		//大砲の場合
		if (ShotFlag == true) {
			if (vx >= -1.8f) {
				vx += GravityX;
			}
			position.x += vx + FirstSpeedX;
		}
	}

	//もとに戻る
	if (position.y <= -200.0f) {
		onGround = true;
		vy = 0.0f;
		vx = 0.0f;
		position = { -150.0f,0.0f,0.0f };
		AddSpeed = 0.0f;
	}
	//デバッグテキスト
	if (onGround == true) {
		DebugText::GetInstance()->Print("PUSH SPACE", 900, 600, 1);
	}
	DebugText::GetInstance()->Print("PlayerSetting by ImGui", 900, 650, 1);
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	object3d->SetPosition(position);
	// 行列の更新など
	object3d->Update();
}

//描画
void Player::Draw() {
	ImGui::Begin("test");
	if (ImGui::TreeNode("Debug")) {
		if (ImGui::TreeNode("Player")) {
			ImGui::SliderFloat("AddSpeed", &AddSpeed, 2, 0);
			if (ImGui::Button("ShotFlag")) {
				if (ShotFlag == false) {
					ShotFlag = true;
				}
				else {
					ShotFlag = false;
				}
			}

			if (ShotFlag == true) {
				ImGui::Text("Shot!");
			}
			else {
				ImGui::Text("No Shot");
			}
			ImGui::Unindent();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
ImGui::End();
	Object3d::PreDraw();
	object3d->Draw();
}

void Player::Finalize()
{
	delete object3d;
}


void Player::OnCollision(const CollisionInfo& info)
{
	DebugText::GetInstance()->Print("Collision detected.",0,0,10);

	for (int i = 0; i < 1; ++i) {

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		ParticleManager::GetInstance()->Add(10, XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
	}
}
