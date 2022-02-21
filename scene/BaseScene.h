#pragma once
#include "DirectXCommon.h"
//�O���錾
class SceneManager;
//�V�[���C���^�[�t�F�[�X
class BaseScene
{
public:
	//���z�f�X�g���N�^
	virtual ~BaseScene() = default;

	//������
	virtual void Initiallize(DirectXCommon* dxCommon) = 0;
	//�J��
	virtual void Finalize() = 0;
	//�X�V
	virtual void Update(DirectXCommon* dxCommon) = 0;
	//�`��
	virtual void Draw(DirectXCommon* dxCommon) = 0;
};