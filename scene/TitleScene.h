#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "BaseScene.h"
/// �^�C�g���V�[��
class TitleScene : public BaseScene{
public:
	/// ������
	void Initiallize() override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update() override;
	/// �`��
	void Draw() override;

private://�����o�ϐ�
	Sprite* sprite = { nullptr };
};

