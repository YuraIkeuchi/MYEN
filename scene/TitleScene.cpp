#include "TitleScene.h"
#include "Audio.h"
#include "input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void TitleScene::Initiallize() {
	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/180_20220130123604.png");

	//背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	//スプライト生成
}

void TitleScene::Update() {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->Button_A) || input->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	DebugText::GetInstance()->Print("ENTER to GAMEPLAY!!", 200, 100, 1.0f);
}

void TitleScene::Draw() {
	sprite->PreDraw();
	//背景用
	sprite->Draw();
}


void TitleScene::Finalize() {
	//３ｄのモデルのデリート
	delete sprite;
}

