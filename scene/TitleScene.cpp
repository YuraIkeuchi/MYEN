#include "TitleScene.h"
#include "Audio.h"
#include "input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "GamePlayScene.h"

void TitleScene::Initiallize(DirectXCommon* dxCommon) {
	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/2d/sceneback/180_20220130123604.png");

	//背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	//スプライト生成
}

void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_RETURN) || input->TriggerButton(input->Button_B)) {
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	DebugText::GetInstance()->Print("ENTER to GAMEPLAY!!", 200, 100, 1.0f);
}

void TitleScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	sprite->PreDraw();
	//背景用
	sprite->Draw();
	dxCommon->PostDraw();
}


void TitleScene::Finalize() {
	//３ｄのモデルのデリート
	delete sprite;
}

