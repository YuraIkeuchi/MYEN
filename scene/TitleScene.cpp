#include "TitleScene.h"
#include "Audio.h"
#include "input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "GamePlayScene.h"

void TitleScene::Initiallize(DirectXCommon* dxCommon) {
	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/180_20220130123604.png");

	//�w�i�X�v���C�g����
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	//�X�v���C�g����
}

void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_SPACE) || input->TriggerButton(input->Button_B)) {
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw(DirectXCommon* dxCommon) {
	sprite->PreDraw();
	//�w�i�p
	sprite->Draw();
}


void TitleScene::Finalize() {
	//�R���̃��f���̃f���[�g
	delete sprite;
}

