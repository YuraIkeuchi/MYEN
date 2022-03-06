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
	modelPlayer = Model::LoadFromOBJ("chr_sword");
	modelFighter = Model::LoadFromOBJ("chr_sword");

	for (int i = 0; i < Max; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
		EnemySpeed[i] = (float)(rand() % 360);
		Enemyscale[i] = 0;
		Enemyradius[i] = EnemySpeed[i] * PI / 180.0f;
		EnemyCircleX[i] = cosf(Enemyradius[i]) * Enemyscale[i];
		EnemyCircleZ[i] = sinf(Enemyradius[i]) * Enemyscale[i];
		EnemyPosition[i].x = EnemyCircleX[i];
		EnemyPosition[i].y = EnemyCircleZ[i];
		
		enemy[i]->SetPosition(EnemyPosition[i]);
		EnemyAlive[i] = 0;
		EnemyTimer[i] = 100;
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

	objFighter->SetPosition(FighterPosition);
	objFighter->SetScale({ 0.7,0.7,0.7 });
	objPlayer->SetPosition(PlayerPosition);
	objPlayer->SetScale({ 0.7,0.7,0.7 });
	objGround->SetPosition({ 0, -10, 0 });

	// カメラ注視点をセット
	camera->SetTarget(TexPosition);
	camera->SetEye({0,0,0});

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
	TexPosition.x = PlayerCircleX;
	TexPosition.y = PlayerCircleZ;

	//プレイヤー(幻想)
	fantasyradius = fantasySpeed * PI / 180.0f;
	fantasyCircleX = cosf(fantasyradius) * fantasyscale;
	fantasyCircleZ = sinf(fantasyradius) * fantasyscale;
	FantasyPosition.x = fantasyCircleX;
	FantasyPosition.y = fantasyCircleZ;
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
			TexPosition.x = PlayerCircleX;
			TexPosition.y = PlayerCircleZ;
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
				InitTexPosition = TexPosition;
				frame = 0;
				moveNumver = 1;
			}
		}
	}

	if (moveNumver == 1) {
		AttackSpeed = 2 * easeInSine(frame / frameMax);
		if (frame != frameMax) {
					frame = frame + 1;
		}
		angleX = (TexPosition.x - FantasyPosition.x);
		angleZ = (TexPosition.y - FantasyPosition.y);
		angleR = sqrt((TexPosition.x - FantasyPosition.x) * (TexPosition.x - FantasyPosition.x)
			+ (TexPosition.y - FantasyPosition.y) * (TexPosition.y - FantasyPosition.y));
		TexPosition.x -= (angleX / angleR) * AttackSpeed;
		TexPosition.y -= (angleZ / angleR) * AttackSpeed;
		PlayerSpeed = fantasySpeed;
		if (collision->SphereCollision(TexPosition.x, TexPosition.y, TexPosition.z, 0.3, FantasyPosition.x, FantasyPosition.y, FantasyPosition.z, 0.3) == true) {
			fantasyFlag = false;
			moveNumver = 0;
			AttackSpeed = 0.0f;
			Playerradius = PlayerSpeed * PI / 180.0f;
			PlayerCircleX = cosf(Playerradius) * Playerscale;
			PlayerCircleZ = sinf(Playerradius) * Playerscale;
			TexPosition.x = PlayerCircleX;
			TexPosition.y = PlayerCircleZ;
		}
	}

	//プレイヤー(幻想)
	fantasyradius = fantasySpeed * PI / 180.0f;
	fantasyCircleX = cosf(fantasyradius) * fantasyscale;
	fantasyCircleZ = sinf(fantasyradius) * fantasyscale;
	FantasyPosition.x = fantasyCircleX;
	FantasyPosition.y = fantasyCircleZ;
	
	objFighter->SetPosition(TexPosition);
	objPlayer->SetPosition(FantasyPosition);
	
	if (input->PushKey(DIK_SPACE) || input->TriggerButton(input->Button_A)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		Audio::GetInstance()->StopWave(0);
	}
	for (int i = 0; i < Max; i++) {
		hit[i] = collision->SphereCollision(TexPosition.x, TexPosition.y, TexPosition.z, 0.8, EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 0.8);
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
			if (Enemyscale[i] != 10.0f) {
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
		EnemyPosition[i].x = EnemyCircleX[i];
		EnemyPosition[i].y = EnemyCircleZ[i];
		enemy[i]->SetPosition(EnemyPosition[i]);
	}

	if (sizeof(enemy) > 2) {//配列のサイズ確認
		for (int colA = 0; colA < Max; colA++) {
			for (int colB = 1; colB < Max; colB++) {
				if (Collision::CheckSphere2Sphere(enemy[colA]->collider, enemy[colB]->collider) == true && colA != colB) {//当たり判定と自機同士の当たり判定の削除
					DebugText::GetInstance()->Print("Hit", 0, 0, 5.0f);
					enemy[colA]->SetRotation({ 0,(float)90.0f * colB,0 });
					enemy[colB]->SetRotation({ 0,180,0 });
					break;
				}
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
	//titleTexture->SetPosition(TexPosition);
	//titleTexture->Update(camera->GetViewMatrix(), camera->GetViewProjectionMatrix());
	//fantasyTexture->SetPosition(FantasyPosition);
	//fantasyTexture->Update(camera->GetViewMatrix(), camera->GetViewProjectionMatrix());

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
			ImGui::SliderFloat("Player", &TexPosition.x, 50, -50);
			ImGui::SliderFloat("Player", &TexPosition.y, 50, -50);
			ImGui::SliderFloat("fantasy", &FantasyPosition.x, 50, -50);
			ImGui::SliderFloat("fantasy", &FantasyPosition.y, 50, -50);
			ImGui::SliderFloat("Player", &pos.x, 50, -50);
			ImGui::SliderFloat("Player", &pos.y, 50, -50);
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
		if (EnemyAlive[i] == 1) {
			enemy[i]->Draw();
		}
	}

	//object1->Draw(dxCommon->GetCmdList());
	//objSkydome->Draw();
	//objGround->Draw();
	objFighter->Draw();
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