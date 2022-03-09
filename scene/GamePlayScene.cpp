#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "FbxLoader.h"
#include"Texture.h"
float easeInSine(float x) {
	return x * x * x;
}
float easeOutBack(float x) {
	return x == 1 ? 1 : 1 - powf(2, -10 * x);
}
void GamePlayScene::Initiallize(DirectXCommon* dxCommon)
{
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	// テクスチャ1番に読み込み
	Sprite::LoadTexture(1, L"Resources/BackGround.png");

	Audio::GetInstance()->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	Audio::GetInstance()->LoopWave(0, 0.3);
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);

	//spriteBG->SetSize({ 1280,720 });
	// 3Dオブジェクト生成
	objPlayer = Object3d::Create();
	objFighter = Object3d::Create();
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objAllow = Object3d::Create();
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelPlayer = Model::LoadFromOBJ("chr_sword");
	modelFighter = Model::LoadFromOBJ("chr_knight");
	modelAllow = Model::LoadFromOBJ("Arrow");
	for (int i = 0; i < Max; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
		enemy[i]->SetPosition({ 0.0f,0.0,2.0 });
	}
	////普通のテクスチャ(板ポリ)
	//Texture::LoadTexture(0, L"Resources/Title.png");
	//titleTexture = Texture::Create(0, { 0,0,0 }, { 2,2,2 }, { 1,1,1,1 });
	//titleTexture->TextureCreate();
	//titleTexture->SetPosition(TexPosition);
	//titleTexture->SetScale({ 0.5,0.5,0.5 });

	////普通のテクスチャ(板ポリ)
	//Texture::LoadTexture(2, L"Resources/Title.png");
	//fantasyTexture = Texture::Create(2, { 0,0,0 }, { 2,2,2 }, { 1,1,1,0.5 });
	//fantasyTexture->TextureCreate();
	//fantasyTexture->SetPosition(FantasyPosition);
	//fantasyTexture->SetScale({ 0.5,0.5,0.5 });
	//Texture::LoadTexture(1, L"Resources/ダウンロード.png");
	//
	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objPlayer->SetModel(modelPlayer);
	objFighter->SetModel(modelFighter);
	objAllow->SetModel(modelAllow);
	objFighter->SetPosition(FighterPosition);
	objFighter->SetScale({ 3.0,3.0,3.0 });
	objPlayer->SetPosition(PlayerPosition);
	objPlayer->SetScale({ 1.0,1.0,1.0 });
	objGround->SetPosition({ 0, 0, 0 });
	objAllow->SetScale({ 1.0f,2.0f,2.0f });
	objAllow->SetRotation(ArrowRotation);
	objAllow->SetPosition({ PlayerPosition.x,PlayerPosition.y,PlayerPosition.z + 3.0f });
	// カメラ注視点をセット
	camera->SetTarget(PlayerPosition);
	camera->SetEye({PlayerPosition.x,PlayerPosition.y + 10,PlayerPosition.z - 10});

	// モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// カメラをセット
	FBXObject3d::SetCamera(camera);
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	object1 = new FBXObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	
}

void GamePlayScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	DebugText* debugText = DebugText::GetInstance();
	lightGroup->Update();
	if ((input->PushKey(DIK_DOWN)) || (input->TiltStick(input->Right))) {
		PlayerPosition.x += 0.3f;
	}

	if ((input->PushKey(DIK_DOWN)) || (input->TiltStick(input->Left))) {
		PlayerPosition.x -= 0.3f;
	}

	if ((input->PushKey(DIK_DOWN)) || (input->TiltStick(input->Up))) {
		PlayerPosition.z += 0.3f;
	}

	if ((input->PushKey(DIK_DOWN)) || (input->TiltStick(input->Down))) {
		PlayerPosition.z -= 0.3f;
	}

	if (input->PushButton(input->Button_RB)) {
		ButtunFlag = true;
	} else {
		ButtunFlag = false;
	}

	if (input->PushButton(input->Button_B)) {
		ArrowRotation.y++;
	}

	if (input->PushButton(input->Button_Y)) {
		ArrowRotation.y--;
	}



	// カメラ注視点をセット
	camera->SetTarget(PlayerPosition);
	camera->SetEye({ PlayerPosition.x,PlayerPosition.y + 10,PlayerPosition.z - 10 });

	object1->Update();
	camera->Update();
	objPlayer->SetPosition(PlayerPosition);
	objPlayer->Update();
	objFighter->SetPosition(FighterPosition);
	objFighter->Update();
	objSkydome->Update();
	objGround->Update();
	objAllow->SetRotation(ArrowRotation);
	objAllow->SetPosition({ PlayerPosition.x,PlayerPosition.y,PlayerPosition.z + 5.0f });
	objAllow->Update();

	for (int i = 0; i < Max; i++) {
		enemy[i]->Update();
	}
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	
	ImGui::Begin("test");
	if (ImGui::TreeNode("Debug"))
	{
		if (ImGui::TreeNode("Player"))
		{
			ImGui::SliderFloat("Player", &PlayerPosition.x, 50, -50);
			ImGui::SliderFloat("Player", &PlayerPosition.y, 50, -50);
			ImGui::Unindent();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Enemy"))
		{
			ImGui::SliderFloat("Enemyscale0", &EnemyPosition[0].x, 50, -50);
			ImGui::SliderFloat("Enemyscale1", &EnemyPosition[0].y, 50, -50);
			ImGui::SliderFloat("Enemyscale2", &EnemyPosition[1].x, 50, -50);
			ImGui::SliderFloat("EnemySpeed0", &EnemyPosition[1].y, 50, -50);
			ImGui::SliderFloat("EnemySpeed0", &EnemySpeed[0], 50, -50);
			ImGui::SliderFloat("EnemySpeed1", &EnemySpeed[1], 50, -50);
			ImGui::Unindent();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw();
	// 背景スプライト描画
	spriteBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	//dxCommon->ClearDepthBuffer();
	Texture::PreDraw(dxCommon->GetCmdList());
	//if (fantasyFlag == true) {
	//	fantasyTexture->Draw();
	//}
	//titleTexture->Draw();
	//for (int i = 0; i < Max; i++) {
	//	if (EnemyAlive[i] == 1) {
	//		//enemyTexture[i]->Draw();
	//	}
	//}
	Texture::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw();
	if (fantasyFlag == true) {
		objPlayer->Draw();
	}
	//背景用
	for (int i = 0; i < Max; i++) {
			enemy[i]->Draw();
	}

	object1->Draw(dxCommon->GetCmdList());
	objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objPlayer->Draw();
	if (ButtunFlag == true) {
		objAllow->Draw();
	}
	Object3d::PostDraw();
#pragma endregion

	// 前景スプライト描画前処理
	Sprite::PreDraw();

}

void GamePlayScene::Finalize()
{
	//スプライト開放
	delete spriteBG;
	//オブジェクト開放
	delete objPlayer;
	delete objFighter;
	delete fantasyTexture;
	delete titleTexture;
	for (int i = 0; i < Max; i++) {
		delete enemyTexture[i];
	}
	//モデル開放
	delete modelFighter;
	delete modelPlayer;
}