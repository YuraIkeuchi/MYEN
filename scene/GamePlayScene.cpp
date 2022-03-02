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
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelPlayer = Model::LoadFromOBJ("senntouki3");
	modelFighter = Model::LoadFromOBJ("chr_sword");
	//普通のテクスチャ(板ポリ)
	Texture::LoadTexture(0, L"Resources/Title.png");
	titleTexture = Texture::Create(0, { 0,0,0 }, { 2,2,2 }, { 1,1,1,1 });
	titleTexture->TextureCreate();
	titleTexture->SetPosition(texpo);
	titleTexture->SetScale({ 0.5,0.5,0.5 });

	//普通のテクスチャ(板ポリ)
	Texture::LoadTexture(2, L"Resources/Title.png");
	fantasyTexture = Texture::Create(2, { 0,0,0 }, { 2,2,2 }, { 1,1,1,0.5 });
	fantasyTexture->TextureCreate();
	fantasyTexture->SetPosition(fantasypos);
	fantasyTexture->SetScale({ 0.5,0.5,0.5 });
	Texture::LoadTexture(1, L"Resources/ダウンロード.png");
	for (int i = 0; i < Max; i++) {
		EnemySpeed[i] = (float)(rand() % 360);
		Enemyscale[i] = (float)(rand() % 15);
		//プレイヤー
		Enemyradius[i] = EnemySpeed[i] * PI / 180.0f;
		EnemyCircleX[i] = cosf(Enemyradius[i]) * Enemyscale[i];
		EnemyCircleZ[i] = sinf(Enemyradius[i]) * Enemyscale[i];
		enepos[i].x = EnemyCircleX[i];
		enepos[i].y = EnemyCircleZ[i];
		
		enemyTexture[i] = Texture::Create(1, { 0,0,0 }, { 2,2,2 }, { 1,1,1,1 });
		enemyTexture[i]->TextureCreate();
		enemyTexture[i]->SetPosition(enepos[i]);
		enemyTexture[i]->SetScale({ 0.3,0.3,0.3 });
	}
	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objPlayer->SetModel(modelPlayer);
	objFighter->SetModel(modelFighter);

	objFighter->SetPosition(FighterPosition);
	objPlayer->SetPosition(PlayerPosition);
	objGround->SetPosition({ 0, -10, 0 });

	// カメラ注視点をセット
	camera->SetTarget(texpo);
	camera->SetEye(cameraPos);
	/*lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->SetPointLightActive(0, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;*/
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
	//プレイヤー
	Playerradius = PlayerSpeed * PI / 180.0f;
	PlayerCircleX = cosf(Playerradius) * Playerscale;
	PlayerCircleZ = sinf(Playerradius) * Playerscale;
	texpo.x = PlayerCircleX;
	texpo.y = PlayerCircleZ + 20;

	//プレイヤー(幻想)
	fantasyradius = fantasySpeed * PI / 180.0f;
	fantasyCircleX = cosf(fantasyradius) * fantasyscale;
	fantasyCircleZ = sinf(fantasyradius) * fantasyscale;
	fantasypos.x = fantasyCircleX;
	fantasypos.y = fantasyCircleZ + 20;
}

void GamePlayScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	DebugText* debugText = DebugText::GetInstance();
	lightGroup->Update();

	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->TiltStick(input->Up)) {
		PlayerSpeed += 2.1f;
	}
	if (input->PushKey(DIK_DOWN) || input->TiltStick(input->Down)) {
		PlayerSpeed -= 2.1f;
	}
	/*if (PlayerSpeed >= 360 || PlayerSpeed <= -360) {
		PlayerSpeed = 0;
	}*/

	//プレイヤー
	Playerradius = PlayerSpeed * PI / 180.0f;
	PlayerCircleX = cosf(Playerradius) * Playerscale;
	PlayerCircleZ = sinf(Playerradius) * Playerscale;
	texpo.x = PlayerCircleX;
	texpo.y = PlayerCircleZ - 5;

	if (input->TriggerKey(DIK_0)) {
		if (fantasyFlag == false) {
			fantasyFlag = true;
			fantasySpeed = PlayerSpeed;
		} else {

		}
	}

	//プレイヤー(幻想)
	fantasyradius = fantasySpeed * PI / 180.0f;
	fantasyCircleX = cosf(fantasyradius) * fantasyscale;
	fantasyCircleZ = sinf(fantasyradius) * fantasyscale;
	fantasypos.x = fantasyCircleX;
	fantasypos.y = fantasyCircleZ - 5;

	objPlayer->SetPosition(PlayerPosition);
	objFighter->SetPosition(FighterPosition);

	if (input->PushKey(DIK_SPACE) || input->TriggerButton(input->Button_A)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		Audio::GetInstance()->StopWave(0);
	}

	bool hit = collision->SphereCollision(PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 0.5, FighterPosition.x, FighterPosition.y, FighterPosition.z, 0.5);

	if (hit) {
		debugText->Print("Hit", 5.0, 5.0, 5.0f);
	}

	//if (input->PushKey(DIK_0)) {
	//	object1->PlayAnimation();
	//}

	object1->Update();
	//{
	//	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	//	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	//	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	//}
	camera->SetEye(cameraPos);
	camera->Update();
	objPlayer->Update();
	objFighter->Update();
	objSkydome->Update();
	objGround->Update();
	titleTexture->SetPosition(texpo);
	titleTexture->Update(camera->GetViewMatrix(), camera->GetViewProjectionMatrix());
	fantasyTexture->SetPosition(fantasypos);
	fantasyTexture->Update(camera->GetViewMatrix(), camera->GetViewProjectionMatrix());
	for (int i = 0; i < Max; i++) {
		enemyTexture[i]->SetPosition(enepos[i]);
		enemyTexture[i]->Update(camera->GetViewMatrix(), camera->GetViewProjectionMatrix());
	}
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//ImGui::Begin("Light");
	////ImGui::SetWindowPos(ImVec2(0, 0));
	////ImGui::SetWindowSize(ImVec2(500, 200));
	////ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	////ImGui::InputFloat3("lightDir0", lightDir0);
	////ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	////ImGui::InputFloat3("lightDir1", lightDir1);
	////ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	////ImGui::InputFloat3("lightDir2", lightDir2);
	////ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);
	//ImGui::ColorEdit3("pointLightColor", pointLightColor, ImGuiColorEditFlags_Float);
	//ImGui::InputFloat3("pointLightPos", pointLightPos);
	//ImGui::InputFloat3("pointLightAtten", pointLightAtten);
	//ImGui::End();

	ImGui::Begin("test");
	if (ImGui::TreeNode("Debug"))
	{
		if (ImGui::TreeNode("Player"))
		{
			ImGui::SliderFloat("PlayerSpeed", &PlayerSpeed, 50, -50);
			ImGui::SliderFloat("FantasySpeed", &fantasySpeed, 50, -50);
			//ImGui::Text("HP,%d", PlayerHP);
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
	if (fantasyFlag == true) {
		fantasyTexture->Draw();
	}
	titleTexture->Draw();
	for (int i = 0; i < Max; i++) {
		enemyTexture[i]->Draw();
	}
	Texture::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw();
	//object1->Draw(dxCommon->GetCmdList());
	//objSkydome->Draw();
	//objGround->Draw();
	//objFighter->Draw();
	//objPlayer->Draw();
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