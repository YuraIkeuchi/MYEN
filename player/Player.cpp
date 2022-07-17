#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ModelManager.h"
using namespace DirectX;

Player::Player() {
	model = ModelManager::GetInstance()->GetModel(ModelManager::Player);
	//object3d = new Object3d();
}

bool Player::Initialize()
{
	scale = { 1.2f,1.2f,1.2f };
	pos.y = -5.0f;
	pos.z = -5.0f;
	rotation.y = 90.0f;
	Object3d* object3d_ = new Object3d();
	object3d_ = Object3d::Create();
	object3d_->SetModel(model);
	object3d_->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	
	//object3d_->SetRotation({ 0.0f,90.0f,0.0f });
	object3d_->SetPosition(pos);
	object3d_->SetRotation(rotation);
	object3d_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	object3d_->SetScale(scale);
	object3d.reset(object3d_);

	Object3d* Toon_object3d_ = new Object3d();
	Toon_object3d_ = Object3d::Create();
	Toon_object3d_->SetModel(model);
	Toon_object3d_->CreateGraphicsPipeline(L"Resources/shaders/ToonVS.hlsl", L"Resources/shaders/ToonPS.hlsl");
	position = { 100.0f,-60.0,0.0f };
	//object3d_->SetRotation({ 0.0f,90.0f,0.0f });
	Toon_object3d_->SetPosition(pos);
	Toon_object3d_->SetRotation(rotation);
	Toon_object3d_->SetScale(scale);
	Toon_object3d.reset(Toon_object3d_);

	Object3d* Single_object3d_ = new Object3d();
	Single_object3d_ = Object3d::Create();
	Single_object3d_->SetModel(model);
	Single_object3d_->CreateGraphicsPipeline(L"Resources/shaders/SingleColorVS.hlsl", L"Resources/shaders/SingleColorPS.hlsl");
	position = { 100.0f,-60.0,0.0f };
	//object3d_->SetRotation({ 0.0f,90.0f,0.0f });
	Single_object3d_->SetPosition(pos);
	Single_object3d_->SetRotation(rotation);
	Single_object3d_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	Single_object3d_->Update();
	Single_object3d_->SetScale(scale);
	Single_object3d.reset(Single_object3d_);
	
	// コライダーの追加
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	//object3d->Update();
	// A,Dで旋回
	if (input->PushKey(DIK_A)) {
		pos.x -= 0.3f;
	} else if (input->PushKey(DIK_D)) {
		pos.x += 0.3f;
	}

	// 移動ベクトルをY軸周りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	//座標移動
	if (input->LeftTiltStick(input->Right)) {
		pos.x += 0.1f;
	}

	if (input->LeftTiltStick(input->Left)) {
		pos.x -= 0.1f;
	}

	if (input->LeftTiltStick(input->Up)) {
		pos.z += 0.1f;
	}

	if (input->LeftTiltStick(input->Down)) {
		pos.z -= 0.1f;
	}

	//回転
	if (input->PushButton(input->Button_RB)) {
		rotation.y += 1.0f;
	}

	if (input->PushButton(input->Button_LB)) {
		rotation.y -= 1.0f;
	}
	
	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		pos.x += fallV.m128_f32[0];
		pos.y += fallV.m128_f32[1];
		pos.z += fallV.m128_f32[2];
	}
	// ジャンプ操作
	else if (input->TriggerButton(input->Button_A)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}

	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISIONSHAPE_MESH);
	// 交差による排斥分動かす
	pos.x += callback.move.m128_f32[0];
	pos.y += callback.move.m128_f32[1];
	pos.z += callback.move.m128_f32[2];
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// 着地
			onGround = true;
			pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}

	if (pos.y <= 1.0f) {
		onGround = true;
		pos.y = 1.0f;
	}

	object3d->SetColor(color);
	object3d->SetAddOffset(addoffset);
	object3d->SetRotation(rotation);
	object3d->SetPosition(pos);
	// 行列の更新など
	object3d->Update();

	Toon_object3d->SetColor(color);
	Toon_object3d->SetAddOffset(addoffset);
	Toon_object3d->SetRotation(rotation);
	Toon_object3d->SetPosition(pos);
	Toon_object3d->Update();

	Single_object3d->SetColor(color);
	Single_object3d->SetAddOffset(addoffset);
	Single_object3d->SetRotation(rotation);
	Single_object3d->SetPosition(pos);
	Single_object3d->Update();
}

//描画
void Player::Draw(int DrawNumber) {
	Object3d::PreDraw();
	if (DrawNumber == 0) {
		object3d->Draw();
	}
	else if (DrawNumber == 1) {
		Toon_object3d->Draw();
	}
	else if (DrawNumber == 2) {
		Single_object3d->Draw();
	}
}

void Player::Finalize()
{
	//delete object3d;
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

void Player::ImGuiDraw() {
	ImGui::Begin("color");
	ImGui::SetWindowPos(ImVec2(0, 500));
	ImGui::SetWindowSize(ImVec2(280, 240));
	ImGui::SliderFloat("color.r", &color.x, 1, 0);
	ImGui::SliderFloat("color.g", &color.y, 1, 0);
	ImGui::SliderFloat("color.b", &color.z, 1, 0);
	ImGui::SliderFloat("color.a", &color.w, 1, 0);
	ImGui::End();
}