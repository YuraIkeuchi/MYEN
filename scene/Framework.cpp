#include "Framework.h"
//#include "FbxLoader.h"
void Framework::Run() 
{
	Initialize();
	while (true) {
		//�X�V����
		Update();
		//�I�����N�G�X�g�������甲����
		if (endRequst) {
			break;
		}
		//�`�揈��
		Draw();
	}
	//�������
	Finalize();

}
void Framework::Initialize()
{
	winApp = new WinApp();
	winApp->Initialize();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	// ���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);

	audio = Audio::GetInstance();
	audio->Initialize();
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);
	const int debugTextTexNumber = 0;
	Sprite::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// ���C�g�ÓI������
	LightGroup::StaticInitialize(dxCommon->GetDev());
	// �f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	Object3d::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// FBX�֘A�ÓI������
	//FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

}

void Framework::Finalize()
{

	delete dxCommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update()
{
	//�E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		//�Q�[�����[�v�I��
		endRequst = true;
		return;
	}

	input->Update();
	
	//�V�[���X�V����
	SceneManager::GetInstance()->Update();
}

void Framework::Draw()
{
	dxCommon->PreDraw();
	//�V�[���`��
	SceneManager::GetInstance()->Draw();
	//�Ńo�b�N�e�L�X�g�̕`��
	debugText->DrawAll();

	dxCommon->PostDraw();

}