#pragma once
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "BaseScene.h"
#include "DebugCamera.h"
#include "LightGroup.h"
#include "FBXObject3d.h"
#include "DirectXCommon.h"
#include "DirectXMath.h"
#include "Texture.h"
#include "Enemy.h"
#include "ParticleManager.h"
#include "PostEffect.h"
#include "ModelManager.h"
#include <vector>
#include <memory>
#include <list> // �w�b�_�t�@�C���C���N���[�h
using namespace std;         //  ���O��Ԏw��
//�O���錾
class SceneManager;
class CollisionManager;
class Player;
class TouchableObject;
//�V�[���C���^�[�t�F�[�X
class BaseScene
{
public:
	//void CreateParticles();
	///�Q�[���V�[���p
	DebugCamera* camera = nullptr;
	Sprite* spriteBG = nullptr;
	Sprite* spritePlayerHP = nullptr;
	Sprite* spriteBossHP = nullptr;
	std::vector<Object3d*> objects;
	Player* player = nullptr;
	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;
	ParticleManager* particleMan = nullptr;
	XMFLOAT3 PlayerPosition = { 1.0f,5.0f,0.0f };
	//�����蔻�� ���C
	Ray ray;
	Texture* titleTexture = nullptr;
	//�����蔻��
	Collision* collision = nullptr;
	LightGroup* lightGroup = nullptr;
	XMFLOAT3 cameraPos = { 0, 3.0f, -20.0f };
	Enemy* enemy = nullptr;
	//Player* player = nullptr;
	CollisionManager* collsionManager = nullptr;
	PostEffect* postEffect = nullptr;
	Model* modelFloor = nullptr;
	unique_ptr <Object3d> objFloor;
	Model* modelSphere = nullptr;
	unique_ptr <Object3d> objSphere = nullptr;
	Model* modelSkydome = nullptr;
	unique_ptr <Object3d> objSkydome = nullptr;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	//�X�|�b�g���C�g
	float spotLightDir[3] = { 0,0,1 };
	float spotLightPos[3] = { 0,0,-2 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f };

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };
	int PostType = 0;

	enum PostType {
		Normal,
		Blur,
		Inversion,
	};
public:
	//���z�f�X�g���N�^
	virtual ~BaseScene() = default;

	//������
	virtual void Initiallize(DirectXCommon* dxCommon) = 0;
	//�J��
	virtual void Finalize() = 0;
	//�X�V
	virtual void Update(DirectXCommon* dxCommon) = 0;
	//�`��
	virtual void Draw(DirectXCommon* dxCommon) = 0;
};