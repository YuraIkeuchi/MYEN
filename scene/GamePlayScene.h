#pragma once
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
const int Max = 3;
//�Q�[���v���C�V�[��
class GamePlayScene : public BaseScene
{
private://�����o�ϐ�

public:
	//������
	void Initiallize(DirectXCommon* dxCommon) override;
	//�X�V
	void Update(DirectXCommon* dxCommon) override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;
	//�J��
	void Finalize() override;
	///�Q�[���V�[���p
	DebugCamera* camera = nullptr;
	Sprite* spriteBG = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelPlayer = nullptr;
	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;
	Object3d* objPlayer = nullptr;
	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;
	
	Texture* titleTexture = nullptr;
	Texture* fantasyTexture = nullptr;
	Texture* enemyTexture[Max] = { nullptr };

	XMFLOAT3 PlayerPosition = { -5.0f,0.0f,0.0f };
	XMFLOAT3 FighterPosition = { 5.0f,0.0f,0.0f };
	XMFLOAT3 FloorPosition = { 0.0f,-2.0f,0.0f };
	XMFLOAT3 texpo = { 0,0,0 };
	XMFLOAT3 fantasypos = { 0,0,0 };
	XMFLOAT3 enepos[Max];
	
	//�����蔻�� ��
	Sphere sphere;
	//�����蔻�� ����
	Plane plane;
	//�����蔻�� �O�p�`
	Triangle triangle;
	//�����蔻�� ���C
	Ray ray;
	//�����蔻��
	Collision* collision = nullptr;
	LightGroup* lightGroup = nullptr;
	XMFLOAT3 cameraPos = { 0, 3.0f, -20.0f };
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	float PI = 3.14f;
	float Playerradius = 0.0f;
	float PlayerSpeed = 0.0f;
	float Playerscale = 20.0f;// LaneNum�ƈꏏ�ɕς��邱��
	float PlayerCircleX = 0.0f;
	float PlayerCircleZ = 0.0f;

	float fantasyradius = 0.0f;
	float fantasySpeed = 0.0f;
	float fantasyscale = 20.0f;// LaneNum�ƈꏏ�ɕς��邱��
	float fantasyCircleX = 0.0f;
	float fantasyCircleZ = 0.0f;
	bool fantasyFlag = false;
	//�G�֌W
	float Enemyradius[Max] = { 0.0f };
	float EnemySpeed[Max] = { 0.0f };
	float Enemyscale[Max] = { 0.0f };// LaneNum�ƈꏏ�ɕς��邱��
	float EnemyCircleX[Max] = { 0.0f };
	float EnemyCircleZ[Max] = { 0.0f };
};