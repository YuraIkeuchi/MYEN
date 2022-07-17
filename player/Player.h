#pragma once
#include "Object3d.h"
#include"Model.h"
#include <memory>
#include <list> // �w�b�_�t�@�C���C���N���[�h
using namespace std;         //  ���O��Ԏw��
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
	void Draw(int DrawNumber);

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
	const XMFLOAT3& GetPosition() { return  pos; }

	const XMFLOAT3& GetRotation() { return object3d->GetRotation(); }

	void SetPosition(XMFLOAT3 position) { object3d->SetPosition(position); }

	void SetRotation(XMFLOAT3 rotation) { object3d->SetRotation(rotation); }

private:
	XMFLOAT3 pos = { 0,-2,0 };
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	unique_ptr <Object3d> object3d;
	unique_ptr <Object3d> Toon_object3d;
	unique_ptr <Object3d> Single_object3d;
	Model* model;
	int hit = 0;
	bool onGround = true;
	// �����x�N�g��
	DirectX::XMVECTOR fallV;
	enum Shader {
		normal,
		one,
	};

	int Shadernumber = 0;
};

