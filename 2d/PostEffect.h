#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	
	//�R���X�g���N�^
	PostEffect();

	//������
	void Initialize();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

private://�����o�ϐ�
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
};