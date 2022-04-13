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

	void SetPosition(XMFLOAT3 position) { object3d->SetPosition(position); }

	void SetRotation(XMFLOAT3 rotation) { object3d->SetRotation(rotation); }

private:
	XMFLOAT3 pos = { 0,40,0 };
	Object3d* object3d;
	Model* model;
	int hit = 0;
	bool onGround = true;
	// �����x�N�g��
	DirectX::XMVECTOR fallV;
	const float Gravity = -0.7f / 60.0f;
	float v = 0.0f;
	int imageTimer = 0;
};

