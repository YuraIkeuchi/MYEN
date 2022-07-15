#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class PipelineManager
{
public: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //�����o�ϐ�
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> m_pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> m_rootSignature;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	PipelineManager(ID3D12Device* dev);

	/// <summary>
	/// ADS�V�F�[�_�[
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	void CreateADSPipeline(ID3D12Device* dev);
	
	/// <summary>
	/// �g�D�[���V�F�[�_�[
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	void CreateToonPipeline(ID3D12Device* dev);

	/// <summary>
	/// �P�F�V�F�[�_�[
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	void CreateMonochromaticPipeline(ID3D12Device* dev);

	/// <summary>
	/// �p�C�v���C���X�e�[�g���擾
	/// </summary>
	ID3D12PipelineState *GetPipelineState() { return m_pipelineState.Get(); }

	/// <summary>
	/// ���[�g�V�O�l�`�����擾
	/// </summary>
	ID3D12RootSignature *GetRootSignature() { return m_rootSignature.Get(); }
};
