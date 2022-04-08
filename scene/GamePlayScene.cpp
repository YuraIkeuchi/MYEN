#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "FbxLoader.h"
#include"Texture.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "TouchableObject.h"
#include "MeshCollider.h"

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
	collsionManager = CollisionManager::GetInstance();
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
	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	//// 3Dオブジェクト生成
	//player = new Player();
	//player->Initialize();
	player = new Player();
	player->Initialize();

	//enemy = new Enemy();
	//enemy->Initialize();

	objSkydome = Object3d::Create();
	
	objSphere = Object3d::Create();

	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("StartMap");
	modelSphere = Model::LoadFromOBJ("sphere");
	modelPlane = Model::LoadFromOBJ("plane1x1");
	modelBox = Model::LoadFromOBJ("box1x1x1");
	modelPyramid = Model::LoadFromOBJ("pyramid1x1");

	modelFighter = Model::LoadFromOBJ("chr_sword");
	objSkydome->SetModel(modelSkydome);
	objGround = TouchableObject::Create(modelGround);
	objSphere->SetModel(modelSphere);
	
	objGround->SetPosition({ 0, 0, 0 });
	objSkydome->SetPosition({ 0, 0, 0 });
	objSphere->SetPosition({ 0, 1, 0 });

	//Model* modeltable[10] = {
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelBox,
	//	modelPyramid,
	//};

	//const int DIV_NUM = 10;
	//const float LAND_SCALE = 3.0f;
	//for (int i = 0; i < DIV_NUM; i++) {
	//	for (int j = 0; j < DIV_NUM; j++) {

	//		int modelIndex = rand() % 10;

	//		TouchableObject* object = TouchableObject::Create(modeltable[modelIndex]);
	//		object->SetScale(LAND_SCALE);
	//		object->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE, 0, (i - DIV_NUM / 2) * LAND_SCALE });
	//		objects.push_back(object);
	//	}
	//}

	//objFighter->SetPosition(PlayerPosition);
	// コライダーの追加
	objSphere->SetCollider(new SphereCollider);

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
	
	//// カメラ注視点をセット
	//camera->SetTarget(player->GetPosition());
	//camera->SetEye({ player->GetPosition().x,player->GetPosition().y + 10,player->GetPosition().z - 10 });

		// カメラ注視点をセット
	camera->SetEye({ 0,5,-20 });
	camera->SetTarget({ 0, 1, 0 });
	/*camera->SetDistance(3.0f);*/
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
	if (input->PushKey(DIK_0)) {
		object1->PlayAnimation();
	}
	object1->Update();
	objSkydome->Update();
	objGround->Update();
	objSphere->Update();
	
	player->Update();
	//enemy->Update();
	particleMan->Update();
	camera->Update();

	Ray ray;
	//ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	//ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	////ゲームオーバーに行く
	//if (PlayerHP == 0) {
	//	SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	//}

	////ゲームクリア
	//if (BossHP <= 0) {
	//	SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	//}
	camera->SetEye({ 0,15,-20 });
	camera->SetTarget({ 0, 1, 0 });
	// 全ての衝突をチェック
	collsionManager->CheckAllCollisions();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{

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
		object1->Draw(dxCommon->GetCmdList());
		objSkydome->Draw();
		objGround->Draw();
		player->Draw();
		//objFighter->Draw();
		//enemy->Draw();
		objSphere->Draw();
		Object3d::PostDraw();
		// パーティクルの描画
		particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

		// 前景スプライト描画前処理
		Sprite::PreDraw();

}

void GamePlayScene::Finalize()
{
	//スプライト開放
	delete spriteBG;
}

//void GamePlayScene::CreateParticles()
//{
//	for (int i = 0; i < 10; i++) {
//		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
//		const float rnd_pos = 10.0f;
//		XMFLOAT3 pos{};
//		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
//		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
//		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
//
//		const float rnd_vel = 0.1f;
//		XMFLOAT3 vel{};
//		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
//		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
//		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
//
//		XMFLOAT3 acc{};
//		const float rnd_acc = 0.001f;
//		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
//
//		// 追加
//		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
//	}
//}
