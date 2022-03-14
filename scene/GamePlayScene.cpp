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

float easeInOut(float x) {
	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
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

	//プレイヤーの行動
	if (ArmMoveNumber <= 1) {
		//移動
		if ((input->PushKey(DIK_RIGHT)) || (input->LeftTiltStick(input->Right)) && PlayerAttackFlag == false && AttackMoveNumber == 0) {
			PlayerPosition.x += PlayerMoveSpeed;
			PlayerRotation.y = 90;
			ArmSpeed = 0;
		}

		if ((input->PushKey(DIK_LEFT)) || (input->LeftTiltStick(input->Left)) && PlayerAttackFlag == false && AttackMoveNumber == 0) {
			PlayerPosition.x -= PlayerMoveSpeed;
			PlayerRotation.y = 270;
			ArmSpeed = 180;
		}

		if ((input->PushKey(DIK_UP)) || (input->LeftTiltStick(input->Up)) && PlayerAttackFlag == false && AttackMoveNumber == 0) {
			PlayerPosition.z += PlayerMoveSpeed;
			PlayerRotation.y = 0;
			ArmSpeed = 90;
		}

		if ((input->PushKey(DIK_DOWN)) || (input->LeftTiltStick(input->Down)) && PlayerAttackFlag == false && AttackMoveNumber == 0) {
			PlayerPosition.z -= PlayerMoveSpeed;
			PlayerRotation.y = 180;
			ArmSpeed = 270;
		}

		if (input->PushButton(input->Button_RB) && PlayerAttackFlag == false && EnemyWeight <= 7.0f && AttackMoveNumber == 0) {
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
		//攻撃
		if (input->TriggerButton(input->Button_A) && PlayerAttackFlag == false && EnemyWeight != 0.0f) {
			PlayerAttackFlag = true;
			AttackMoveNumber = 1;
			initScale = Armscale;
			initSpeed = ArmSpeed;
			InitPlayerRotation = PlayerRotation;
			frame2 = 0;
			frame3 = 0;
			Savescale = EnemyWeight / 2;
			frameMax2 += EnemyWeight * 10;
			frameMax3 += EnemyWeight * 10;
		}

		if (input->TriggerCrossKey(input->Cross_Up) == true) {
			if (SpeedWeight == false) {
				SpeedWeight = true;
				PlayerMoveSpeed = PlayerMoveSpeed - (EnemyWeight / 150);
			} else {
				SpeedWeight = false;
				PlayerMoveSpeed = 0.3f;
			}
		}

		if (input->TriggerCrossKey(input->Cross_Down) == true) {
			if (AttackWeight == false) {
				AttackWeight = true;
			} else {
				AttackWeight = false;
			}
		}
	}

	//攻撃
	if (PlayerAttackFlag == true) {
		ArmSpeed = initSpeed + 360.0f * easeInOut(frame2 / frameMax2);
		PlayerRotation.y = InitPlayerRotation.y - 360.0f * easeInOut(frame2 / frameMax2);
		if (frame2 <= frameMax2) {
			frame2 = frame2 + 1;
		} else {
			PlayerAttackFlag = false;
			frameMax2 = 80.0f;
		}
	}

	if (AttackMoveNumber == 1) {
		Armscale = initScale + Savescale * easeInOut(frame3 / frameMax3);
		if (frame3 <= frameMax3) {
			frame3 = frame3 + 1;
		} else {
			AttackMoveNumber = 2;
			initScale = Armscale;
			frame3 = 0;
			frameMax3 = 20.0f;
		}
	}

	else if (AttackMoveNumber == 2) {
		Armscale = initScale - Savescale * easeInOut(frame3 / frameMax3);
		if (frame3 <= frameMax3) {
			frame3 = frame3 + 1;
		} else {
			AttackMoveNumber = 0;
			Armscale = 1.0f;
			frameMax3 = 80.0f;
		}
	}

	if (input->PushButton(input->Button_B)) {
		ArrowRotation.y++;
	}

	if (input->PushButton(input->Button_Y)) {
		ArrowRotation.y--;
	}

	//腕を伸ばす
	if (ArmMoveNumber == 2) {
		Armscale = initScale + 3.0f * easeOutBack(frame / frameMax);
		if (frame != frameMax) {
			frame = frame + 1;
		} else {
			ArmMoveNumber = 3;
			frame = 0;
			initScale = Armscale;
		}
	}

	else if (ArmMoveNumber == 3) {
		Armscale = initScale - 3.0f * easeOutBack(frame / frameMax);
		if (frame != frameMax) {
			frame = frame + 1;
		} else {
			ArmMoveNumber = 0;
			initScale = Armscale;
		}
	}

	//各当たり判定
	//プレイヤーと敵の当たり判定
	for (int i = 0; i < Max; i++) {
		if (collision->SphereCollision(PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 0.3,
			EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 0.3) == true && EnemyAlive[i] == 1
			&& EnemyCatch[i] == false) {
			EnemyAlive[i] = 0;
			PlayerHP--;
		}
	}
	//敵と腕の当たり判定(くっつける)
	for (int i = 0; i < Max; i++) {
		if (collision->SphereCollision(ArmPosition.x, ArmPosition.y, ArmPosition.z, 0.5,
			EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 0.5) == true && EnemyAlive[i] == 1
			&& ArmMoveNumber >= 2 && EnemyCatch[i] == false && EnemyWeight < 7.0f) {
			EnemyCatch[i] = true;
			EnemyWeight += 1.0f;
			if (SpeedWeight == true) {
				PlayerMoveSpeed = PlayerMoveSpeed - (EnemyWeight / 100);
			}
		}

		if (EnemyCatch[i] == true) {
			EnemyPosition[i] = ArmPosition;
		}
	}
	//腕とボスの当たり判定
	if (collision->SphereCollision(ArmPosition.x, ArmPosition.y, ArmPosition.z, 0.5,
		FighterPosition.x, FighterPosition.y, FighterPosition.z, 0.5) == true &&
		PlayerAttackFlag == true) {
		BossHit = true;
		HitTimer = 10;
		frame2 = frameMax2;
		PlayerAttackFlag = false;
		PlayerMoveSpeed = 0.3f;
		for (int i = 0; i < Max; i++) {
			if (EnemyCatch[i] == true) {
				EnemyCatch[i] = false;
				EnemyAlive[i] = 0;
			}
		}
	}

	//腕と敵の当たり判定(攻撃時)
	for (int i = 0; i < Max; i++) {
		if (collision->SphereCollision(ArmPosition.x, ArmPosition.y, ArmPosition.z, 0.5,
			EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 0.5) == true &&
			PlayerAttackFlag == true && EnemyAlive[i] == 1 && EnemyCatch[i] == false) {
			EnemyAlive[i] = 0;
			PlayerAttackFlag = false;
			frame2 = frameMax2;

			PlayerMoveSpeed = 0.3f;
			if (EnemyWeight != 0.0f) {
				EnemyWeight = 0.0f;
			}
			for (int j = 0; j < Max; j++) {
				if (EnemyCatch[j] == true) {
					EnemyAlive[j] = 0;
				}
			}
		}
	}

	//ボスダメージ判定
	if (BossHit == true) {
		HitTimer--;
		if (HitTimer == 0) {
			BossHP -= (EnemyWeight * 2);
			EnemyWeight = 0.0f;
			BossHit = false;
		}
	}

	//追ってくる範囲
	for (int i = 0; i < Max; i++) {
		if (collision->SphereCollision(PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 5.0,
			EnemyPosition[i].x, EnemyPosition[i].y, EnemyPosition[i].z, 5.0) == true && EnemyAlive[i] == 1 && EnemyCatch[i] == false) {
			CircleInFlag[i] = 1;
		} else {
			CircleInFlag[i] = 0;
		}

		if (CircleInFlag[i] == 1) {
			EnemyangleX[i] = (PlayerPosition.x - 	EnemyPosition[i].x);
			EnemyangleZ[i] = (PlayerPosition.z - 	EnemyPosition[i].z);
			EnemyangleR[i] = sqrt((PlayerPosition.x - EnemyPosition[i].x) * (PlayerPosition.x - EnemyPosition[i].x)
				+ (PlayerPosition.z - EnemyPosition[i].z) * (PlayerPosition.z - EnemyPosition[i].z));
			EnemyPosition[i].x += (EnemyangleX[i] / EnemyangleR[i]) * 0.05;
			EnemyPosition[i].z += (EnemyangleZ[i] / EnemyangleR[i]) * 0.05;
		}
		enemy[i]->SetPosition(EnemyPosition[i]);
	}

	for (int i = 0; i < Max; i++) {
		if (EnemyAlive[i] == 0) {
			EnemyTimer[i]--;
		}

		if (EnemyTimer[i] == 0) {
			EnemyAlive[i] = 1;
			EnemyTimer[i] = 100;
			EnemyCatch[i] = false;
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

	//攻撃するかどうか
	if (PlayerAttackFlag == false) {
		DebugText::GetInstance()->Print("B to ATTACK!!", 200, 100, 1.0f);
	}
	//敵の重みでスピードが落ちるかどうか
	if (SpeedWeight == false) {
		DebugText::GetInstance()->Print("NoAdd SpeedWeight", 200, 120, 1.0f);
	} else {
		DebugText::GetInstance()->Print("YesAdd SpeedWeight", 200, 120, 1.0f);
	}

	//敵の重みでスピードが落ちるかどうか
	if (AttackWeight == false) {
		DebugText::GetInstance()->Print("NoAdd AttackWeight", 200, 140, 1.0f);
	} else {
		DebugText::GetInstance()->Print("YesAdd AttackWeight", 200, 140, 1.0f);
	}

	if (EnemyWeight < 7.0f) {
		DebugText::GetInstance()->Print("RB to Catch!!", 200, 160, 1.0f);
	} else {
		DebugText::GetInstance()->Print("Can not Catch!!", 200, 160, 1.0f);
	}

	//ゲームオーバーに行く
	if (PlayerHP == 0) {
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}

	//ゲームクリア
	if (BossHP <= 0) {
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}
	//腕の場所調整
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
			ImGui::SliderFloat("Armscale", &Armscale, 50, -50);
			ImGui::SliderFloat("ArmSpeed", &ArmSpeed, 50, -50);
			ImGui::SliderFloat("Weight", &EnemyWeight, 50, -50);
			ImGui::SliderFloat("PlayerMove", &PlayerMoveSpeed, 50, -50);
			ImGui::SliderFloat("SaveScale", &Savescale, 50, -50);
			ImGui::Unindent();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Boss"))
		{
			ImGui::Text("BossHP:%d", BossHP);
			ImGui::Unindent();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Enemy"))
		{
			ImGui::Text("Alive:%d", EnemyAlive[0]);
			ImGui::Text("Alive:%d", EnemyAlive[1]);
			ImGui::Text("Catch:%d", EnemyCatch[0]);
			ImGui::Text("Catch:%d", EnemyCatch[1]);
			ImGui::Unindent();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Frame"))
		{
			ImGui::SliderFloat("frame2", &frame2, 50, -50);
			ImGui::SliderFloat("frame3", &frame3, 50, -50);
			ImGui::SliderFloat("frameMax2", &frameMax2, 50, -50);
			ImGui::SliderFloat("frameMax3", &frameMax3, 50, -50);
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

	//object1->Draw(dxCommon->GetCmdList());
	objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objPlayer->Draw();
	if (ButtunFlag == true) {
		//objAllow->Draw();
	}
	objArm->Draw();
	Object3d::PostDraw();
#pragma endregion

	// 前景スプライト描画前処理
	Sprite::PreDraw();
	spritePlayerHP->Draw();
	spritePlayerHP->SetSize({ (float)(PlayerHP * 30),20 });
	spriteBossHP->Draw();
	spriteBossHP->SetSize({ (float)(BossHP * 20),20 });
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
	//モデル開放
	delete modelFighter;
	delete modelPlayer;
	delete modelAllow;
	delete modelArm;
}