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
#include "ImageManager.h"
#include <Easing.h>

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
	//Audio::GetInstance()->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	//Audio::GetInstance()->LoopWave(0, 0.3f);
	// �w�i�X�v���C�g����
	
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���ǂ݂���
	Sprite::LoadTexture(100, L"Resources/2d/white1x1.png");
	//�|�X�g�G�t�F�N�g�̏�����
	//(����)
	postEffect = new PostEffect();
	postEffect->Initialize();
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/PostEffectTestPS.hlsl");
	//�K�E�V�A��
	/*gaussian = new PostEffect();
	gaussian->Initialize();
	gaussian->CreateGraphicsPipeline(L"Resources/Shaders/GaussianVS.hlsl", L"Resources/Shaders/GaussianPS.hlsl");*/
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	////// 3D�I�u�W�F�N�g����
	Player* _player = new Player();
	_player->Initialize();
	player.reset(_player);
	
	//�X�e�[�W��
	Object3d* objFloor_ = new Object3d();
	objFloor_ = Object3d::Create();
	modelFloor = Model::LoadFromOBJ("ground");
	objFloor_->SetModel(modelFloor);
	objFloor_->SetPosition({ 0, -1, 0 });
	objFloor_->SetScale({ 6.0f,1.0f,6.0f });
	objFloor_->SetAddOffset(0.01f);
	objFloor_->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	objFloor.reset(objFloor_);
	// ���f���ǂݍ���
	modelSphere = Model::LoadFromOBJ("sphere", true);

	// 3D�I�u�W�F�N�g����
	Object3d* objSphere_ = new Object3d();
	objSphere_ = Object3d::Create();
	objSphere_->SetModel(modelSphere);
	objSphere_->SetPosition(m_SpherePos1);
	objSphere_->SetScale({ 3.0f,3.0f,3.0f });
	objSphere_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	objSphere_->SetAddOffset(0.04f);
	objSphere_->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	//objSphere_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	objSphere.reset(objSphere_);

	// 3D�I�u�W�F�N�g����
	Object3d* objSphere2_ = new Object3d();
	objSphere2_ = Object3d::Create();
	objSphere2_->SetModel(modelSphere);
	objSphere2_->SetPosition(m_SpherePos2);
	objSphere2_->SetScale({ 3.0f,3.0f,3.0f });
	objSphere2_->SetColor({ 0.0f,1.0f,0.0f,1.0f });
	objSphere2_->SetAddOffset(0.04f);
	objSphere2_->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	//objSphere_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	objSphere2.reset(objSphere2_);


	// 3D�I�u�W�F�N�g����
	Object3d* objSphere3_ = new Object3d();
	objSphere3_ = Object3d::Create();
	objSphere3_->SetModel(modelSphere);
	objSphere3_->SetPosition(m_SpherePos3);
	objSphere3_->SetScale({ 3.0f,3.0f,3.0f });
	objSphere3_->SetColor({ 0.0f,0.0f,1.0f,1.0f });
	objSphere3_->SetAddOffset(0.04f);
	objSphere3_->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	//objSphere_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	objSphere3.reset(objSphere3_);
	
	modelSkydome = ModelManager::GetInstance()->GetModel(ModelManager::Skydome);
	
	
	Object3d* objskydome_ = new Object3d();
	objskydome_ = Object3d::Create();
	objskydome_->SetModel(modelSkydome);
	objskydome_->SetPosition({ 0, 0, 0 });
	objskydome_->SetScale({ 2.0f,2.0f,2.0f });
	objskydome_->SetAddOffset(0.07f);
	objskydome_->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
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

	//�e�N�X�`���֌W
	Texture* LineTexture_ = Texture::Create(ImageManager::Line, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	LineTexture_->TextureCreate();
	//DushEffecttexture->SetRotation({ 90,0,0 });
	LineTexture_->SetScale({0.3f,0.1f,1.0f});
	LineTexture_->SetPosition(LinePos);
	//LineTexture_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	LineTexture.reset(LineTexture_);

	//�e�N�X�`���֌W
	Texture* BoxTexture_ = Texture::Create(ImageManager::Box, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	BoxTexture_->TextureCreate();
	//DushEffecttexture->SetRotation({ 90,0,0 });
	BoxTexture_->SetScale({ 0.1f,0.1f,1.0f });
	BoxTexture_->SetPosition(BoxPos);
	BoxTexture.reset(BoxTexture_);


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
	//objSphere->SetColor({ 0.0f,0.0f,1.0f,0.0f });
	objSphere->Update();
	objSphere2->Update();
	objSphere3->Update();
	objSkydome->Update();
	LineTexture->Update();
	BoxTexture->Update();
	///�|�C���g���C�g
	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	//
	//float radius = speed * 3.14f / 180.0f;

	////XMFLOAT3 pos = object3d2->GetPosition();
	////XMFLOAT3 center = object3d->GetPosition();

	//CirclePos.x = cos(radius) * scale;
	//CirclePos.y = sin(radius) * scale;

	//m_SpherePos1.x = CirclePos.x + m_SpherePos2.x;
	//m_SpherePos1.y = CirclePos.y + m_SpherePos2.y;
	if (MathStart)
	{
		if (m_frame < 1.0f) {
			m_frame += 0.01f;
		}
		else {
			m_frame = 1.0f;
			MathStart = false;
		}
	}

	if (input->TriggerKey(DIK_SPACE)) {
		MathStart = true;
	}

	if (input->TriggerKey(DIK_R))
	{
		m_frame = 0.0f;
		m_SpherePos1 = { -100,40,150 };
		m_SpherePos2 = { -100,0,150 };
		m_SpherePos3 = { -100, -40,150 };
	}

	////���̈ړ�
	//if (input->LeftTiltStick(input->Right)) {
	//	LinePos.x += 0.2f;
	//}

	//if (input->LeftTiltStick(input->Left)) {
	//	LinePos.x -= 0.2f;
	//}

	//if (input->LeftTiltStick(input->Up)) {
	//	LinePos.y += 0.2f;
	//}

	//if (input->LeftTiltStick(input->Down)) {
	//	LinePos.y -= 0.2f;
	//}

	////�O�ϓ����蔻��
	//Sphere box;
	//box.center = { BoxPos.x,BoxPos.y,BoxPos.z };
	//box.radius = 1;

	//Box line;
	//line.center = { LinePos.x,LinePos.y,LinePos.z };
	//line.scale = { 2.5f,0.8f,1.0f };

	//if (Collision::CheckSphere2Box(box, line)) {
	//	BoxTexture->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	//}
	//else {
	//	BoxTexture->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	//}

	
	//if (MathStart) {
	//	//�{�[�����
	//	{
	//		m_velY1 += m_gravity1;    //�X�s�[�h�ɏd�͂����Z�����

	//		//if (input->TriggerKey(DIK_1)) {
	//		//	Bound = true;
	//		//	velX *= -1.0f;
	//		//}

	//		m_SpherePos1.x += m_velX1;
	//		//m_SpherePos1.y += m_velY1;

	//		m_velX1 *= m_damp1;    //velX������
	//	}

	//	//�{�[�����
	//	{
	//		m_velY2 += m_gravity2;    //�X�s�[�h�ɏd�͂����Z�����

	//		//if (input->TriggerKey(DIK_1)) {
	//		//	Bound = true;
	//		//	velX *= -1.0f;
	//		//}

	//		m_SpherePos2.x -= m_velX2;
	//		//m_SpherePos2.y += m_velY2;

	//		m_velX2 *= m_damp2;    //velX������
	//	}


	//	//���Z�b�g
	//	if (!m_Bound1 && !m_Bound2) {
	//		if (m_velX1 <= 0.01f && m_velX2 <= 0.01f) {
	//			Reset = true;
	//			MathStart = false;
	//		}
	//	}
	//	else if(m_Bound1 && m_Bound2) {
	//		if (m_velX1 >= -0.01f && m_velX2 >= -0.01f) {

	//			Reset = true;
	//			MathStart = false;
	//			m_Bound1 = false;
	//			m_Bound2 = false;
	//		}
	//	}

	//}
	//else {
	//	if (input->TriggerKey(DIK_1)) {
	//		MathStart = true;
	//	}
	//}


	//if (Reset) {
	//	if (input->TriggerKey(DIK_0)) {

	//		m_SpherePos1 = { -70,0,150 };
	//		m_velX1 = 3.0f;
	//		m_SpherePos2 = { 70,0,150 };
	//		m_velX2 = 8.0f;
	//		Reset = false;
	//	}
	//}


	//if (collision->CircleCollision(m_SpherePos1.x, m_SpherePos1.y, 2.0f, m_SpherePos2.x, m_SpherePos2.y, 2.0f)) {

	//	m_Bound1 = true;
	//	m_velX1 *= -1.0f;
	//	m_Bound2 = true;
	//	m_velX2 *= -1.0f;
	//}
	m_SpherePos1.x = Ease(In, Cubic, m_frame, m_SpherePos1.x, 100.0f);
	m_SpherePos2.x = Ease(In, SoftBack, m_frame, m_SpherePos2.x, 100.0f);
	m_SpherePos3.x = Ease(In, Linear, m_frame, m_SpherePos3.x, 100.0f);
	objSphere->SetPosition(m_SpherePos1);
	objSphere2->SetPosition(m_SpherePos2);
	objSphere3->SetPosition(m_SpherePos3);
	LineTexture->SetPosition(LinePos);
	BoxTexture->SetPosition(BoxPos);
	
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
	ChangePostEffect(PostType);
	camera->SetEye({ 0,0,-10 });
	camera->SetTarget({ 0, 0, 0 });
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
		player->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		dxCommon->SetFullScreen(true);
			ImGuiDraw();
		GameDraw(dxCommon);
		player->ImGuiDraw();
		dxCommon->PostDraw();
	}
}

void GamePlayScene::Finalize()
{
	//�X�v���C�g�J��
	delete spriteBG;
	delete postEffect;
	//delete gaussian;
	//player->Finalize();
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
	//objSkydome->Draw();
	//objFloor->Draw();
	objSphere->Draw();
	objSphere2->Draw();
	objSphere3->Draw();
	//player->Draw(MaterialNumber);
	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	Texture::PreDraw();
	/*LineTexture->Draw();
	BoxTexture->Draw();*/
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
	{
		//	ImGui::Begin("postEffect");
		//	ImGui::SetWindowPos(ImVec2(1000, 150));
		//	ImGui::SetWindowSize(ImVec2(280, 150));
		//	if (ImGui::RadioButton("PostEffect", &PlayPostEffect)) {
		//		PlayPostEffect = true;
		//	}
		//	if (ImGui::RadioButton("Default", &PlayPostEffect)) {
		//		PlayPostEffect = false;
		//	}
		//	ImGui::End();
		//}
		//{
		//	ImGui::Begin("Material");
		//	ImGui::SetWindowPos(ImVec2(1000, 300));
		//	ImGui::SetWindowSize(ImVec2(280, 150));
		//	if (ImGui::RadioButton("Normal", &MaterialNumber)) {
		//		MaterialNumber = NormalMaterial;
		//		player->SetShaderChange(true);
		//		player->ChangeShader(MaterialNumber);
		//	}
		//	if (ImGui::RadioButton("Toon", &MaterialNumber)) {
		//		MaterialNumber = Toon;
		//		player->SetShaderChange(true);
		//		player->ChangeShader(MaterialNumber);
		//	}
		//	if (ImGui::RadioButton("Single", &MaterialNumber)) {
		//		MaterialNumber = Single;
		//		player->SetShaderChange(true);
		//		player->ChangeShader(MaterialNumber);
		//	}
		//	ImGui::End();
		//}
		//{
		//	if (PlayPostEffect) {
		//		ImGui::Begin("PostType");
		//		ImGui::SetWindowPos(ImVec2(1000, 450));
		//		ImGui::SetWindowSize(ImVec2(280, 150));
		//		if (ImGui::RadioButton("Stripe", &PostType)) {
		//			PostType = Stripe;
		//			m_ChangePostEffect = true;
		//			//ChangePostEffect(PostType);
		//		}
		//		if (ImGui::RadioButton("Gaussian", &PostType)) {
		//			PostType = Blur;
		//			m_ChangePostEffect = true;
		//			
		//		}
		//		ImGui::End();
		//	}
		//}
		{
			ImGui::Begin("Pos");
			ImGui::SetWindowPos(ImVec2(1000, 550));
			ImGui::SetWindowSize(ImVec2(280, 300));
			ImGui::SliderFloat("m_SpherePos1.x", &m_SpherePos1.x, 360, -360);
			ImGui::SliderFloat("m_SpherePos2.x", &m_SpherePos2.x, 360, -360);
			ImGui::SliderFloat("m_SpherePos3.x", &m_SpherePos3.x, 360, -360);
			//ImGui::Text("IsPlay::%d", isFlag);
			ImGui::End();
		}
	}
}

void GamePlayScene::ChangePostEffect(int PostType) {
	if (m_ChangePostEffect) {
		if (PostType == Stripe) {
			postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/PostEffectTestPS.hlsl");
		}
		else if (PostType == Blur) {
			postEffect->CreateGraphicsPipeline(L"Resources/Shaders/GaussianVS.hlsl", L"Resources/Shaders/GaussianPS.hlsl");
		}
		m_ChangePostEffect = false;
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
