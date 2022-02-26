#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "FbxLoader.h"
#include"Texture.h"
void GamePlayScene::Initiallize(DirectXCommon* dxCommon)
{
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	// �e�N�X�`��1�Ԃɓǂݍ���
	Sprite::LoadTexture(1, L"Resources/BackGround.png");

	Audio::GetInstance()->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	Audio::GetInstance()->LoopWave(0, 0.3);
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);

	//spriteBG->SetSize({ 1280,720 });
	// 3D�I�u�W�F�N�g����
	objPlayer = Object3d::Create();
	objFighter = Object3d::Create();
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelPlayer = Model::LoadFromOBJ("senntouki3");
	modelFighter = Model::LoadFromOBJ("chr_sword");
	//���ʂ̃e�N�X�`��(�X�v���C�g����Ȃ���)
	Texture::LoadTexture(0, L"Resources/Title.png");
	titleTexture = Texture::Create(0, { 0,0,0 }, { 12,12,12 }, { 1,1,1,1 });
	titleTexture->TextureCreate();
	titleTexture->SetPosition(texpo);
	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objPlayer->SetModel(modelPlayer);
	objFighter->SetModel(modelFighter);

	objFighter->SetPosition(FighterPosition);
	objPlayer->SetPosition(PlayerPosition);
	objGround->SetPosition({ 0, -10, 0 });

	// �J���������_���Z�b�g
	camera->SetTarget(texpo);
	camera->SetEye(cameraPos);
	/*lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->SetPointLightActive(0, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;*/
	// ���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// �f�o�C�X���Z�b�g
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// �J�������Z�b�g
	FBXObject3d::SetCamera(camera);
	// �O���t�B�b�N�X�p�C�v���C������
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

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		if (input->PushKey(DIK_UP)) {
			cameraPos.y += 0.1f;
		}
		if (input->PushKey(DIK_DOWN)) {
			cameraPos.y -= 0.1f;
		}
		if (input->PushKey(DIK_LEFT)) {
			cameraPos.x -= 0.1f;
		}
		if (input->PushKey(DIK_RIGHT)) {
			cameraPos.x += 0.1f;
		}
		objPlayer->SetPosition(PlayerPosition);
		objFighter->SetPosition(FighterPosition);
	}

	if (input->PushKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	bool hit = collision->SphereCollision(PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 0.5, FighterPosition.x, FighterPosition.y, FighterPosition.z, 0.5);

	if (hit) {
		debugText->Print("Hit", 5.0, 5.0, 5.0f);
	}

	if (input->PushKey(DIK_0)) {
		object1->PlayAnimation();
	}
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
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	ImGui::Begin("Light");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));
	//ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	//ImGui::InputFloat3("lightDir0", lightDir0);
	//ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	//ImGui::InputFloat3("lightDir1", lightDir1);
	//ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	//ImGui::InputFloat3("lightDir2", lightDir2);
	//ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);
	ImGui::ColorEdit3("pointLightColor", pointLightColor, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("pointLightPos", pointLightPos);
	ImGui::InputFloat3("pointLightAtten", pointLightAtten);
	ImGui::End();


#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw();
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	//dxCommon->ClearDepthBuffer();
	Texture::PreDraw(dxCommon->GetCmdList());
	titleTexture->Draw();
	Texture::PostDraw();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();
	object1->Draw(dxCommon->GetCmdList());
	//objSkydome->Draw();
	//objGround->Draw();
	//objFighter->Draw();
	//objPlayer->Draw();
	Object3d::PostDraw();
#pragma endregion

	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw();

}

void GamePlayScene::Finalize()
{
	//�X�v���C�g�J��
	delete spriteBG;
	//�I�u�W�F�N�g�J��
	delete objPlayer;
	delete objFighter;
	delete titleTexture;
	//���f���J��
	delete modelFighter;
	delete modelPlayer;
}