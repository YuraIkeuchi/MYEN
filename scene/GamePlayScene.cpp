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
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	collsionManager = CollisionManager::GetInstance();
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	// �e�N�X�`��1�Ԃɓǂݍ���
	Sprite::LoadTexture(1, L"Resources/BackGround.png");
	Sprite::LoadTexture(2, L"Resources/PlayerHP.png");
	Sprite::LoadTexture(3, L"Resources/PlayerHP.png");
	Audio::GetInstance()->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	Audio::GetInstance()->LoopWave(0, 0.3);
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	spritePlayerHP = Sprite::Create(2, { 0.0f,0.0f });
	spriteBossHP = Sprite::Create(3, { 0.0f,0.0f });

	spriteBossHP->SetColor({ 1.0f,0.0f,0.0f,1.0 });
	spritePlayerHP->SetPosition({ 0.0f,520.0f });
	
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���ǂ݂���
	Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	//// 3D�I�u�W�F�N�g����
	//player = new Player();
	//player->Initialize();
	player = new Player();
	player->Initialize();

	//enemy = new Enemy();
	//enemy->Initialize();

	objSkydome = Object3d::Create();
	
	objSphere = Object3d::Create();

	modelSkydome = Model::LoadFromOBJ("skydome");
	modelBossMap = Model::LoadFromOBJ("BossMap");
	modelFloor = Model::LoadFromOBJ("floor");
	modelSphere = Model::LoadFromOBJ("sphere");
	modelPlane = Model::LoadFromOBJ("plane1x1");
	modelBox = Model::LoadFromOBJ("box1x1x1");
	modelPyramid = Model::LoadFromOBJ("pyramid1x1");

	modelFighter = Model::LoadFromOBJ("chr_sword");
	objSkydome->SetModel(modelSkydome);
	objBossMap = TouchableObject::Create(modelBossMap);
	objFloor = TouchableObject::Create(modelFloor);
	objSphere->SetModel(modelSphere);
	
	objBossMap->SetPosition({ 0, 0, 0 });
	objSkydome->SetPosition({ 0, 0, 0 });
	objSphere->SetPosition({ -5, 1, 0 });
	objFloor->SetScale({ 2.0f,2.0f,2.0f });

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
	// �R���C�_�[�̒ǉ�
	objSphere->SetCollider(new SphereCollider);

	////���ʂ̃e�N�X�`��(�|��)
	//Texture::LoadTexture(0, L"Resources/Title.png");
	//titleTexture = Texture::Create(0, { 0,0,0 }, { 2,2,2 }, { 1,1,1,1 });
	//titleTexture->TextureCreate();
	//titleTexture->SetPosition(TexPosition);
	//titleTexture->SetScale({ 0.5,0.5,0.5 });

	////���ʂ̃e�N�X�`��(�|��)
	//Texture::LoadTexture(2, L"Resources/Title.png");
	//fantasyTexture = Texture::Create(2, { 0,0,0 }, { 2,2,2 }, { 1,1,1,0.5 });
	//fantasyTexture->TextureCreate();
	//fantasyTexture->SetPosition(FantasyPosition);
	//fantasyTexture->SetScale({ 0.5,0.5,0.5 });
	//Texture::LoadTexture(1, L"Resources/�_�E�����[�h.png");
	//
	
	//// �J���������_���Z�b�g
	//camera->SetTarget(player->GetPosition());
	//camera->SetEye({ player->GetPosition().x,player->GetPosition().y + 10,player->GetPosition().z - 10 });

		// �J���������_���Z�b�g
	camera->SetEye({ 0,5,-120 });
	camera->SetTarget({ 0, 1, 0 });
	/*camera->SetDistance(3.0f);*/
	// ���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("Motti_Move");

	// �f�o�C�X���Z�b�g
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// �J�������Z�b�g
	FBXObject3d::SetCamera(camera);
	// �O���t�B�b�N�X�p�C�v���C������
	FBXObject3d::CreateGraphicsPipeline();

	object1 = new FBXObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->SetScale({ 0.03,0.03,0.03 });
	//object1->SetPosition({ 0,0,30 });
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
	objBossMap->Update();
	objFloor->Update();
	objSphere->Update();
	
	player->Update();
	//enemy->Update();
	particleMan->Update();
	camera->Update();

	Ray ray;
	//ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	//ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	////�Q�[���I�[�o�[�ɍs��
	//if (PlayerHP == 0) {
	//	SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	//}

	////�Q�[���N���A
	//if (BossHP <= 0) {
	//	SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	//}
	camera->SetEye({ 0,0,-30 });
	camera->SetTarget({ 0, 0, 0 });
	// �S�Ă̏Փ˂��`�F�b�N
	collsionManager->CheckAllCollisions();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{

	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	//Draw(dxCommon);
	//postEffect->PostDrawScene(dxCommon->GetCmdList());

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
		Sprite::PreDraw();

		postEffect->Draw(dxCommon->GetCmdList());
		// �w�i�X�v���C�g�`��
		//spriteBG->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::PostDraw();
		// �[�x�o�b�t�@�N���A
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

#pragma region 3D�I�u�W�F�N�g�`��
		// 3D�I�u�W�F�N�g�`��O����
		Object3d::PreDraw();
		object1->Draw(dxCommon->GetCmdList());
	/*	objSkydome->Draw();
		objBossMap->Draw();
		objFloor->Draw();
		player->Draw();*/
		//objFighter->Draw();
		//enemy->Draw();
		//objSphere->Draw();
		Object3d::PostDraw();
		// �p�[�e�B�N���̕`��
		//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

		// �O�i�X�v���C�g�`��O����
		Sprite::PreDraw();

}

void GamePlayScene::Finalize()
{
	//�X�v���C�g�J��
	delete spriteBG;
	delete objBossMap;
	delete modelBossMap;
	delete objSkydome;
	delete modelSkydome;
	delete postEffect;
	player->Finalize();
}

//void GamePlayScene::CreateParticles()
//{
//	for (int i = 0; i < 10; i++) {
//		// X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
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
//		// �ǉ�
//		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
//	}
//}
