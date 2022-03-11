#include "GameoverScene.h"
#include "Audio.h"
#include "input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "GamePlayScene.h"
#include "GameoverScene.h"

void GameoverScene::Initiallize(DirectXCommon* dxCommon) {
	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/tittlesprite.png");

	//背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	//スプライト生成
}

void GameoverScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_RETURN) || input->TriggerButton(input->Button_B)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	DebugText::GetInstance()->Print("ENTER to TITLE!!", 200, 100, 1.0f);
}

void GameoverScene::Draw(DirectXCommon* dxCommon) {
	sprite->PreDraw();
	//背景用
	sprite->Draw();
}


void GameoverScene::Finalize() {
	//３ｄのモデルのデリート
	delete sprite;
}

