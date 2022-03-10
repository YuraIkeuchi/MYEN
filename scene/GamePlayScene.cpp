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
	Sprite::LoadTexture(2, L"Resources/PlayerHP.png");
	Sprite::LoadTexture(3, L"Resources/PlayerHP.png");
	Audio::GetInstance()->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	Audio::GetInstance()->LoopWave(0, 0.3);
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	spritePlayerHP = Sprite::Create(2, { 0.0f,0.0f });
	spriteBossHP = Sprite::Create(3, { 0.0f,0.0f });

	spriteBossHP->SetColor({ 1.0f,0.0f,0.0f,1.0 });
	spritePlayerHP->SetPosition({ 0.0f,520.0f });
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
	objArm = Object3d::Create();
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelPlayer = Model::LoadFromOBJ("chr_sword");
	modelFighter = Model::LoadFromOBJ("chr_knight");
	modelAllow = Model::LoadFromOBJ("Arrow");
	modelArm = Model::LoadFromOBJ("Particle");
	for (int i = 0; i < Max; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
		EnemySpeed[i] = (float)(rand() % 360);
		Enemyscale[i] = (float)(rand() % 10 + 20);
		Enemyradius[i] = EnemySpeed[i] * PI / 180.0f;
		EnemyCircleX[i] = cosf(Enemyradius[i]) * Enemyscale[i];
		EnemyCircleZ[i] = sinf(Enemyradius[i]) * Enemyscale[i];
		EnemyPosition[i].x = EnemyCircleX[i] + FighterPosition.x;
		EnemyPosition[i].z = EnemyCircleZ[i] + FighterPosition.z;
		
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
	objAllow->SetModel(modelAllow);
	objArm->SetModel(modelArm);
	objFighter->SetPosition(FighterPosition);
	objFighter->SetScale({ 3.0,3.0,3.0 });
	objPlayer->SetPosition(PlayerPosition);
	objPlayer->SetScale({ 1.0,1.0,1.0 });
	objGround->SetPosition({ 0, 0, 0 });
	objAllow->SetScale({ 1.0f,2.0f,2.0f });
	objAllow->SetRotation(ArrowRotation);
	objAllow->SetPosition({ PlayerPosition.x,PlayerPosition.y,PlayerPosition.z + 3.0f });
	ArmSpeed = 90;
	Armscale = 1.0;
	Armradius = ArmSpeed * PI / 180.0f;
	ArmCircleX = cosf(Armradius) * Armscale;
	ArmCircleZ = sinf(Armradius) * Armscale;
	ArmPosition.x = ArmCircleX + PlayerPosition.x;
	ArmPosition.z = ArmCircleZ + PlayerPosition.z;
	objArm->SetPosition(ArmPosition);
	objArm->SetScale({ 1.0f,1.0f,1.0f });
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
	if (ArmMoveNumber <= 1) {
		if ((input->PushKey(DIK_RIGHT)) || (input->LeftTiltStick(input->Right))) {
			PlayerPosition.x += 0.3f;
			PlayerRotation.y = 90;
			ArmSpeed = 0;
		}

		if ((input->PushKey(DIK_LEFT)) || (input->LeftTiltStick(input->Left))) {
			PlayerPosition.x -= 0.3f;
			PlayerRotation.y = 270;
			ArmSpeed = 180;
		}

		if ((input->PushKey(DIK_UP)) || (input->LeftTiltStick(input->Up))) {
			PlayerPosition.z += 0.3f;
			PlayerRotation.y = 0;
			ArmSpeed = 90;
		}

		if ((input->PushKey(DIK_DOWN)) || (input->LeftTiltStick(input->Down))) {
			PlayerPosition.z -= 0.3f;
			PlayerRotation.y = 180;
			ArmSpeed = 270;
		}

		if (input->PushButton(input->Button_RB)) {
			ButtunFlag = true;
			ArmMoveNumber = 1;
		} else {
			ButtunFlag = false;
			if (ArmMoveNumber == 1) {
				ArmMoveNumber = 2;
				initScale = Armscale;
				frame = 0;
			}
		}

		if (input->TriggerButton(input->Button_A) && PlayerAttackFlag == false) {
			PlayerAttackFlag = true;
			AttackMoveNumber = 1;
			initScale = Armscale;
			initSpeed = ArmSpeed;
			frame2 = 0;
			frame3 = 0;
		}
	}

	if (PlayerAttackFlag == true) {
		ArmSpeed = initSpeed + 720.0f * easeOutBack(frame2 / frameMax2);
		if (frame2 != frameMax2) {
			frame2 = frame2 + 1;
		} else {
			PlayerAttackFlag = false;
		}
	}

	if (AttackMoveNumber == 1) {
		Armscale = initScale + 5.0f * easeOutBack(frame3 / frameMax3);
		if (frame3 != frameMax3) {
			frame3 = frame3 + 1;
		} else {
			AttackMoveNumber = 2;
			initScale = Armscale;
			frame3 = 0;
		}
	}

	else if (AttackMoveNumber == 2) {
		Armscale = initScale - 5.0f * easeOutBack(frame3 / frameMax3);
		if (frame3 != frameMax3) {
			frame3 = frame3 + 1;
		} else {
			AttackMoveNumber = 0;
			initScale = Armscale;
		}
	}

	if (input->PushButton(input->Button_B)) {
		ArrowRotation.y++;
	}

	if (input->PushButton(input->Button_Y)) {
		ArrowRotation.y--;
	}

	if (ArmMoveNumber == 2) {
		Armscale = initScale + 3.0f * easeInSine(frame / frameMax);
		if (frame != frameMax) {
			frame = frame + 1;
		} else {
			ArmMoveNumber = 3;
			frame = 0;
			initScale = Armscale;
		}
	}

	else if (ArmMoveNumber == 3) {
		Armscale = initScale - 3.0f * easeInSine(frame / frameMax);
		if (frame != frameMax) {
			frame = frame + 1;
		} else {
			ArmMoveNumber = 0;
			initScale = Armscale;
		}
	}
	for (int i = 0; i < Max; i++) {
		if (collision->SphereCollision(PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 0.5,
			EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 0.5) == true && EnemyAlive[i] == 1) {
			EnemyAlive[i] = 0;
			PlayerHP--;
		}
	}

	for (int i = 0; i < Max; i++) {
		if (collision->SphereCollision(ArmPosition.x, ArmPosition.y, ArmPosition.z, 0.5,
			EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 0.5) == true && EnemyAlive[i] == 1
			&& ArmMoveNumber >= 2) {
			EnemyPosition[i] = ArmPosition;
		}
	}

	for (int i = 0; i < Max; i++) {
		if (EnemyAlive[i] == 0) {
			EnemyTimer[i]--;
		}

		if (EnemyTimer[i] == 0) {
			EnemyAlive[i] = 1;
			EnemyTimer[i] = 100;

			EnemySpeed[i] = (float)(rand() % 360);
			Enemyscale[i] = (float)(rand() % 10 + 10);
			
		Enemyradius[i] = EnemySpeed[i] * PI / 180.0f;
		EnemyCircleX[i] = cosf(Enemyradius[i]) * Enemyscale[i];
		EnemyCircleZ[i] = sinf(Enemyradius[i]) * Enemyscale[i];
		EnemyPosition[i].x = EnemyCircleX[i] + FighterPosition.x;
		EnemyPosition[i].z = EnemyCircleZ[i] + FighterPosition.z;
		}

		enemy[i]->SetPosition(EnemyPosition[i]);
	}

	Armradius = ArmSpeed * PI / 180.0f;
	ArmCircleX = cosf(Armradius) * Armscale;
	ArmCircleZ = sinf(Armradius) * Armscale;
	ArmPosition.x = ArmCircleX + PlayerPosition.x;
	ArmPosition.z = ArmCircleZ + PlayerPosition.z;

	// カメラ注視点をセット
	camera->SetTarget(PlayerPosition);
	camera->SetEye({ PlayerPosition.x,PlayerPosition.y + 10,PlayerPosition.z - 10 });

	object1->Update();
	camera->Update();
	objPlayer->SetPosition(PlayerPosition);
	objPlayer->SetRotation(PlayerRotation);
	objPlayer->Update();
	objFighter->SetPosition(FighterPosition);
	objFighter->Update();
	objSkydome->Update();
	objGround->Update();
	objAllow->SetRotation(ArrowRotation);
	objAllow->SetPosition({ PlayerPosition.x,PlayerPosition.y,PlayerPosition.z + 5.0f });
	objAllow->Update();
	objArm->SetPosition(ArmPosition);
	objArm->Update();
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
			ImGui::SliderFloat("ArmScale", &Armscale, 50, -50);
			ImGui::SliderFloat("frame3", &frame3, 50, -50);
			ImGui::Text("ArmMove:%d", ArmMoveNumber);
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
	//spriteBG->Draw();

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

	object1->Draw(dxCommon->GetCmdList());
	objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objPlayer->Draw();
	if (ButtunFlag == true) {
		objAllow->Draw();
	}
	objArm->Draw();
	Object3d::PostDraw();
#pragma endregion

	// 前景スプライト描画前処理
	Sprite::PreDraw();
	spritePlayerHP->Draw();
	spritePlayerHP->SetSize({ (float)(PlayerHP * 30),20 });
	spriteBossHP->Draw();
	spriteBossHP->SetSize({ (float)(BossHP * 40),20 });
}

void GamePlayScene::Finalize()
{
	//スプライト開放
	delete spriteBG;
	//オブジェクト開放
	delete objPlayer;
	delete objFighter;
	delete objAllow;
	delete objArm;
	delete fantasyTexture;
	delete titleTexture;
	for (int i = 0; i < Max; i++) {
		delete enemyTexture[i];
	}
	//モデル開放
	delete modelFighter;
	delete modelPlayer;
	delete modelAllow;
	delete modelArm;
}