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
#include "imgui.h"

//float easeInSine(float x) {
//	return x * x * x;
//}
//float easeOutBack(float x) {
//	return x == 1 ? 1 : 1 - powf(2, -10 * x);
//}
//
//float easeInOut(float x) {
//	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
//}
void GamePlayScene::Initiallize(DirectXCommon* dxCommon)
{
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Texture::SetCamera(camera);
	collsionManager = CollisionManager::GetInstance();
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	// �e�N�X�`��1�Ԃɓǂݍ���
	Audio::GetInstance()->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	Audio::GetInstance()->LoopWave(0, 0.3f);
	// �w�i�X�v���C�g����
	
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���ǂ݂���
	Sprite::LoadTexture(100, L"Resources/2d/white1x1.png");
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

	////// 3D�I�u�W�F�N�g����
	////player = new Player();
	////player->Initialize();
	player = new Player();
	player->Initialize();

	//�X�e�[�W��
	Object3d* objFloor_ = new Object3d();
	objFloor_ = Object3d::Create();
	modelFloor = Model::LoadFromOBJ("ground");
	objFloor_->SetModel(modelFloor);
	objFloor_->SetPosition({ 0, -1, 0 });
	objFloor_->SetScale({ 6.0f,1.0f,6.0f });
	objFloor_->CreateGraphicsPipeline(L"Resources/shaders/PointLightVS.hlsl", L"Resources/shaders/PointLightPS.hlsl");
	objFloor.reset(objFloor_);
	// ���f���ǂݍ���
	modelSphere = Model::LoadFromOBJ("sphere", true);

	// 3D�I�u�W�F�N�g����
	Object3d* objSphere_ = new Object3d();
	objSphere_ = Object3d::Create();
	objSphere_->SetModel(modelSphere);
	objSphere_->SetPosition({ -2, 1, 0 });
	objSphere_->CreateGraphicsPipeline(L"Resources/shaders/SingleColorVS.hlsl", L"Resources/shaders/SingleColorPS.hlsl");
	//objSphere_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	objSphere.reset(objSphere_);
	
	modelSkydome = ModelManager::GetInstance()->GetModel(ModelManager::Skydome);
	
	
	Object3d* objskydome_ = new Object3d();
	objskydome_ = Object3d::Create();
	objskydome_->SetModel(modelSkydome);
	objskydome_->SetPosition({ 0, 0, 0 });
	objskydome_->SetScale({ 2.0f,2.0f,2.0f });
	objskydome_->CreateGraphicsPipeline(L"Resources/shaders/PointLightVS.hlsl", L"Resources/shaders/PointLightPS.hlsl");
	objSkydome.reset(objskydome_);
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

	//// �J���������_���Z�b�g
	//camera->SetTarget(player->GetPosition());
	//camera->SetEye({ player->GetPosition().x,player->GetPosition().y + 10,player->GetPosition().z - 10 });


		// �J���������_���Z�b�g
	camera->SetEye({ 0,0,-5 });
	camera->SetTarget({ 0, 1, 0 });
	/*camera->SetDistance(3.0f);*/
	// ���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = ModelManager::GetInstance()->GetFBXModel(ModelManager::Test);

	// �f�o�C�X���Z�b�g
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// �J�������Z�b�g
	FBXObject3d::SetCamera(camera);
	// �O���t�B�b�N�X�p�C�v���C������
	FBXObject3d::CreateGraphicsPipeline();

	object1 = new FBXObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->SetScale({ 0.005f,0.005f,0.005f });

	//�ۉe�̂��߂̂��
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	//lightGroup->SetCircleShadowActive(0, true);
	//�|�C���g���C�g
	/*lightGroup->SetPointLightActive(0, true);

	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;*/

	//lightGroup->SetPointLightActive(0, false);
	//lightGroup->SetPointLightActive(1, false);
	//lightGroup->SetPointLightActive(2, false);
	//lightGroup->SetSpotLightActive(0, true);
	
}

void GamePlayScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	DebugText* debugText = DebugText::GetInstance();
	lightGroup->Update();
	if (input->PushKey(DIK_0)) {
		object1->PlayAnimation();
	}
	object1->SetPosition(player->GetPosition());
	object1->SetRotation(player->GetRotation());

	object1->Update();
	player->Update();
	//enemy->Update();
	particleMan->Update();
	objFloor->Update();
	camera->Update();
	objSphere->Update();
	objSkydome->Update();
	///�|�C���g���C�g
	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	
	/*///�X�|�b�g���C�g
	lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0],spotLightDir[1],spotLightDir[2],0 }));
	lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	lightGroup->SetSpotLightFactorAngle(0, XMFLOAT2(spotLightFactorAngle));*/

	/// <summary>
	///�ۉe
	/// </summary>
	/// <param name="dxCommon"></param>
	/*lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ player->GetPosition().x, player->GetPosition().y, player->GetPosition().z }));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));*/

	
	Ray ray;
	//ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	//ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (input->TriggerButton(input->Button_A)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	////�Q�[���I�[�o�[�ɍs��
	//if (PlayerHP == 0) {
	//	SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	//}

	////�Q�[���N���A
	//if (BossHP <= 0) {
	//	SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	//}
	//cameraPos.x = player->GetPosition().x;
	//cameraPos.y = player->GetPosition().y + 10;
	//cameraPos.z = player->GetPosition().z - 10;
	//camera->SetTarget(player->GetPosition());
	//camera->SetEye(cameraPos);
	//�|�X�g�G�t�F�N�g�̎�ޕύX
	switch (PostType)
	{
	case Stripe://���܂���
		postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/PostEffectTestPS.hlsl");
		break;
	case Blur://�ڂ���
		postEffect->CreateGraphicsPipeline(L"Resources/Shaders/GaussianVS.hlsl", L"Resources/Shaders/GaussianPS.hlsl");
		break;
	default:
		break;
	}
	camera->SetEye({ 0,2,-10 });
	camera->SetTarget({ 0, 2, 0 });
	// �S�Ă̏Փ˂��`�F�b�N
	collsionManager->CheckAllCollisions();
	DebugText::GetInstance()->Print("Raycast Hit.", 0, 30, 10);
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//�`����@
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		GameDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		GameDraw(dxCommon);
		ImGuiDraw();
		dxCommon->PostDraw();
	}
}

void GamePlayScene::Finalize()
{
	//�X�v���C�g�J��
	delete spriteBG;
	delete postEffect;
	player->Finalize();
	//delete objFloor;
	//delete objSphere;
	delete modelFloor;
}

//���f���̕`��
void GamePlayScene::ModelDraw(DirectXCommon* dxCommon) {
#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();
	//object1->Draw(dxCommon->GetCmdList());
	objSkydome->Draw();
	objFloor->Draw();
	//objSphere->Draw();
	player->Draw(MaterialNumber);
	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion
}

//��̕`��ɃX�v���C�g�Ȃǂ�������
void GamePlayScene::GameDraw(DirectXCommon* dxCommon)
{
	//ImGuiDraw();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw();
	// �w�i�X�v���C�g�`��
	//spriteBG->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
	/*Texture::PreDraw();

	Texture::PostDraw();*/

	//�X�v���C�g�̕`��
	ModelDraw(dxCommon);
	//FBX�̕`��
	//object1->Draw(dxCommon->GetCmdList());
}

void GamePlayScene::ImGuiDraw() {
	//{
	//	ImGui::Begin("Light");
	//	ImGui::SetWindowPos(ImVec2(0, 0));
	//	ImGui::SetWindowSize(ImVec2(500, 200));
	//	//ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	//	//ImGui::InputFloat3("lightDir0", lightDir0);
	//	//ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	//	//ImGui::InputFloat3("lightDir1", lightDir1);
	//	//ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	//	//ImGui::InputFloat3("lightDir2", lightDir2);
	//	//ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);
	//	//ImGui::InputFloat3("circleShadowDir", circleShadowDir);
	//	////ImGui::InputFloat3("circleShadowPos", circleShadowPos);
	//	//ImGui::InputFloat3("circleShadowAtten", circleShadowAtten, 8);
	//	//ImGui::InputFloat2("circleShadowFactorAngle", circleShadowFactorAngle);
	//	//ImGui::InputFloat3("fighterPos", fighterPos);
	//	ImGui::ColorEdit3("PointColor", pointLightColor, ImGuiColorEditFlags_Float);
	//	ImGui::InputFloat3("pointLightPos", pointLightPos);
	//	ImGui::InputFloat3("pointLightAtten", pointLightAtten);
	//	ImGui::End();
	//}
	{
		ImGui::Begin("postEffect");
		ImGui::SetWindowPos(ImVec2(1000, 150));
		ImGui::SetWindowSize(ImVec2(280, 150));
		if (ImGui::RadioButton("PostEffect", &PlayPostEffect)) {
			PlayPostEffect = true;
		}
		if (ImGui::RadioButton("Default", &PlayPostEffect)) {
			PlayPostEffect = false;
		}
		ImGui::End();
	}
	{
		ImGui::Begin("Material");
		ImGui::SetWindowPos(ImVec2(1000, 300));
		ImGui::SetWindowSize(ImVec2(280, 150));
		if (ImGui::RadioButton("Normal", &MaterialNumber)) {
			MaterialNumber = NormalMaterial;
		}
		if (ImGui::RadioButton("Toon", &MaterialNumber)) {
			MaterialNumber = Toon;
		}
		if (ImGui::RadioButton("Single", &MaterialNumber)) {
			MaterialNumber = Single;
		}
		ImGui::End();
	}
	{
		if (PlayPostEffect) {
			ImGui::Begin("PostType");
			ImGui::SetWindowPos(ImVec2(1000, 450));
			ImGui::SetWindowSize(ImVec2(280, 150));
			if (ImGui::RadioButton("Stripe", &PostType)) {
				PostType = Stripe;
			}
			if (ImGui::RadioButton("Blur", &PostType)) {
				PostType = Blur;
			}
			ImGui::End();
		}
	}
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
