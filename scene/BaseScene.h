#pragma once

//�O���錾
class SceneManager;
//�V�[���C���^�[�t�F�[�X
class BaseScene
{
public:
	//���z�f�X�g���N�^
	virtual ~BaseScene() = default;

	//������
	virtual void Initiallize() = 0;
	//�J��
	virtual void Finalize() = 0;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() = 0;
};