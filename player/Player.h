#pragma once
#include "Object3d.h"
#include"Model.h"
class Player :
	public Object3d
{
public:
	Player();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	//static Player* Create(Model* model = nullptr);

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �J��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
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
	// �����x�N�g��
	DirectX::XMVECTOR fallV;
	//�d�͉����x
	const float GravityY = -1.2f / 60.0f;
	const float GravityX = -0.4f / 60.0f;
	//����
	float vy = 0.0f;
	float vx = 0.0f;
	//�����x
	float FirstSpeedY = 0.0f;
	float FirstSpeedX = 0.0f;
	//ImGui�p
	float AddSpeed = 0.0f;
	//��C�t���O
	bool ShotFlag = false;
	int imageTimer = 0;
	//���C�֌W
	float Fricrion = 0.5f;
	float FricrionPower = 0.0f;
	float weight = 1000.0f;
	float MoveFricrion = 0.5f;
};

