#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"
/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class Object3d
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X	

	// �p�C�v���C���Z�b�g
	struct PipelineSet
	{
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};

private: // �萔


public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <returns>����</returns>
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height, Camera* camera = nullptr);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();

	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	static void SetLightGroup(LightGroup* lightGroup) {
		Object3d::lightGroup = lightGroup;
	}

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 move);

	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveEyeVector(XMFLOAT3 move);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �e�N�X�`������p�p�C�v���C��
	static PipelineSet pipelineSet;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// �r���{�[�h�s��
	static XMMATRIX matBillboard;
	// Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;
	// �J����
	static Camera* camera;
	// ���C�g
	static LightGroup* lightGroup;


private:// �ÓI�����o�֐�

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

public: // �����o�֐�
	bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="position">�X�P�[��</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	Object3d* parent = nullptr;
	// ���f��
	Model* model = nullptr;
	// �r���{�[�h
	bool isBillboard = false;
};

