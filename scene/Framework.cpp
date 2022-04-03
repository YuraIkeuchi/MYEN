#include "Framework.h"
#include "FbxLoader.h"
void Framework::Run()
{
	Initialize(dxcommon);
	while (true) {
		//�X�V����
		Update(dxcommon);
		//�I�����N�G�X�g�������甲����
		if (endRequst) {
			break;
		}
		//�`�揈��
		Draw(dxcommon);
	}
	//�������
	Finalize();

}
void Framework::Initialize(DirectXCommon* dxCommon)
{
	winApp = new WinApp();
	winApp->Initialize();
	dxcommon = new DirectXCommon();
	dxcommon->Initialize(winApp);
	// ���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);

	audio = Audio::GetInstance();
	audio->Initialize();
	// nullptr�`�F�b�N
	assert(dxcommon);
	assert(input);
	assert(audio);
	const int debugTextTexNumber = 0;
	Sprite::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// ���C�g�ÓI������
	LightGroup::StaticInitialize(dxcommon->GetDev());
	// �f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	Object3d::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	Texture::StaticInitialize(dxcommon->GetDev(), WinApp::window_width, WinApp::window_height);
	// FBX�֘A�ÓI������
	FbxLoader::GetInstance()->Initialize(dxcommon->GetDev());
	// �p�[�e�B�N���}�l�[�W��������
	ParticleManager::GetInstance()->Initialize(dxcommon->GetDev());

}

void Framework::Finalize()
{

	delete dxcommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update(DirectXCommon* dxCommon)
{
	//�E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		//�Q�[�����[�v�I��
		endRequst = true;
		return;
	}

	input->Update();

	//�V�[���X�V����
	SceneManager::GetInstance()->Update(dxCommon);
}

void Framework::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	//�V�[���`��
	SceneManager::GetInstance()->Draw(dxCommon);
	//�Ńo�b�N�e�L�X�g�̕`��
	debugText->DrawAll();

	dxCommon->PostDraw();

}