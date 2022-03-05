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
		Enemyscale[i] = 0;
		Enemyradius[i] = EnemySpeed[i] * PI / 180.0f;
		EnemyCircleX[i] = cosf(Enemyradius[i]) * Enemyscale[i];
		EnemyCircleZ[i] = sinf(Enemyradius[i]) * Enemyscale[i];
		enepos[i].x = EnemyCircleX[i];
		enepos[i].y = EnemyCircleZ[i] - 5;
		
		enemyTexture[i] = Texture::Create(1, { 0,0,0 }, { 2,2,2 }, { 1,1,1,1 });
		enemyTexture[i]->TextureCreate();
		enemyTexture[i]->SetPosition(enepos[i]);
		enemyTexture[i]->SetScale({ 0.3,0.3,0.3 });
		EnemyAlive[i] = 0;
		EnemyTimer[i] = 100;
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
	texpo.y = PlayerCircleZ - 5;

	//プレイヤー(幻想)
	fantasyradius = fantasySpeed * PI / 180.0f;
	fantasyCircleX = cosf(fantasyradius) * fantasyscale;
	fantasyCircleZ = sinf(fantasyradius) * fantasyscale;
	fantasypos.x = fantasyCircleX;
	fantasypos.y = fantasyCircleZ - 5;
}

void GamePlayScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	DebugText* debugText = DebugText::GetInstance();
	lightGroup->Update();

	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->TiltStick(input->Up) || input->PushKey(DIK_DOWN) || input->TiltStick(input->Down)) {
		if (moveNumver == 0) {
			if ((input->PushKey(DIK_UP)) || (input->TiltStick(input->Left))) {
				if (AttackSpeed == 0.0f) {
					PlayerSpeed += 2.0f;
				}
			}
			if ((input->PushKey(DIK_DOWN)) || (input->TiltStick(input->Right))) {
				if (AttackSpeed == 0.0f) {
					PlayerSpeed -= 2.0f;
				}
			}
			//プレイヤー
			Playerradius = PlayerSpeed * PI / 180.0f;
			PlayerCircleX = cosf(Playerradius) * Playerscale;
			PlayerCircleZ = sinf(Playerradius) * Playerscale;
			texpo.x = PlayerCircleX;
			texpo.y = PlayerCircleZ - 5;
		}
	}

	if (input->TriggerKey(DIK_0) || input->TriggerButton(input->Button_B)) {
		if (fantasyFlag == false) {
			fantasyFlag = true;
			fantasySpeed = PlayerSpeed;
		} else {
			if (moveNumver == 0) {
				AttackSpeed = 0;
				initScale = Playerscale;
				initSpeed = PlayerSpeed;
				inittexpo = texpo;
				frame = 0;
				moveNumver = 1;
		
			}
		}
	}

	if (moveNumver == 1) {
		AttackSpeed = 3 * easeInSine(frame / frameMax);
		if (frame != frameMax) {
					frame = frame + 1;
		}
		angleX = (texpo.x - fantasypos.x);
		angleZ = (texpo.y - fantasypos.y);
		angleR = sqrt((texpo.x - fantasypos.x) * (texpo.x - fantasypos.x)
			+ (texpo.y - fantasypos.y) * (texpo.y - fantasypos.y));
		texpo.x -= (angleX / angleR) * AttackSpeed;
		texpo.y -= (angleZ / angleR) * AttackSpeed;
		PlayerSpeed = fantasySpeed;
		if (collision->SphereCollision(texpo.x, texpo.y, texpo.z, 0.5, fantasypos.x, fantasypos.y, fantasypos.z, 0.5) == true) {
			fantasyFlag = false;
			moveNumver = 0;
			AttackSpeed = 0.0f;
			Playerradius = PlayerSpeed * PI / 180.0f;
			PlayerCircleX = cosf(Playerradius) * Playerscale;
			PlayerCircleZ = sinf(Playerradius) * Playerscale;
			texpo.x = PlayerCircleX;
			texpo.y = PlayerCircleZ - 5;
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
	for (int i = 0; i < Max; i++) {
		hit[i] = collision->SphereCollision(texpo.x, texpo.y, texpo.z, 0.8, enepos[i].x, enepos[i].y, enepos[i].z, 0.8);
		if (hit[i]) {
			EnemyAlive[i] = 0;
		}

		if (EnemyAlive[i] == 0) {
			EnemyTimer[i]--;
			if (EnemyTimer[i] == 0) {
				EnemyTimer[i] = 100;
				EnemyAlive[i] = 1;
				EnemySpeed[i] = (float)(rand() % 360);
				Enemyscale[i] = 0;
				EnemyMove[i] = rand() % 2;
			}
		}

		else {
			if (Enemyscale[i] != 20.0f) {
				Enemyscale[i] += 0.5f;;
			} else {
				if (EnemyMove[i] == 1) {
					EnemySpeed[i] += 1.0f;
				} else {
					EnemySpeed[i] -= 1.0f;
				}
			}
		}

		Enemyradius[i] = EnemySpeed[i] * PI / 180.0f;
		EnemyCircleX[i] = cosf(Enemyradius[i]) * Enemyscale[i];
		EnemyCircleZ[i] = sinf(Enemyradius[i]) * Enemyscale[i];
		enepos[i].x = EnemyCircleX[i];
		enepos[i].y = EnemyCircleZ[i] - 5;
	}

	for (int i = 0; i < Max; i++) {
		for (int j = 0; j < Max; j++) {
			if (collision->SphereCollision(enepos[i].x, enepos[i].y, enepos[i].z, 0.8, enepos[!j].x, enepos[!j].y, enepos[!j].z, 0.8) == true) {
				break;
			}
		}
	}

	object1->Update();

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
	
	ImGui::Begin("test");
	if (ImGui::TreeNode("Debug"))
	{
		if (ImGui::TreeNode("Player"))
		{
			ImGui::SliderFloat("Player", &texpo.x, 50, -50);
			ImGui::SliderFloat("Player", &texpo.y, 50, -50);
			ImGui::SliderFloat("fantasy", &fantasypos.x, 50, -50);
			ImGui::SliderFloat("fantasy", &fantasypos.y, 50, -50);
			ImGui::SliderFloat("Player", &pos.x, 50, -50);
			ImGui::SliderFloat("Player", &pos.y, 50, -50);
			ImGui::Unindent();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Enemy"))
		{
			ImGui::SliderFloat("Enemyscale0", &enepos[0].x, 50, -50);
			ImGui::SliderFloat("Enemyscale1", &enepos[0].y, 50, -50);
			ImGui::SliderFloat("Enemyscale2", &enepos[1].x, 50, -50);
			ImGui::SliderFloat("EnemySpeed0", &enepos[1].y, 50, -50);
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
	if (fantasyFlag == true) {
		fantasyTexture->Draw();
	}
	titleTexture->Draw();
	for (int i = 0; i < Max; i++) {
		if (EnemyAlive[i] == 1) {
			enemyTexture[i]->Draw();
		}
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