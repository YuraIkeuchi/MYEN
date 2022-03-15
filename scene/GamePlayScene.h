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
#include "Enemy.h"
const int Max = 15;

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
	Sprite* spritePlayerHP = nullptr;
	Sprite* spriteBossHP = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelPlayer = nullptr;
	Model* modelAllow = nullptr;
	Model* modelArm = nullptr;
	Object3d* objArm = nullptr;
	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;
	Object3d* objPlayer = nullptr;
	Object3d* objAllow = nullptr;
	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;

	XMFLOAT3 PlayerPosition = { -5.0f,0.0f,0.0f };
	XMFLOAT3 PlayerRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 InitPlayerRotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 FighterPosition = { 5.0f,0.0f,0.0f };

	XMFLOAT3 ArmPosition = { 0.0f,0.0f,0.0f };
	XMFLOAT3 EnemyPosition[Max];

	XMFLOAT3 ArrowRotation = { 0,180,0 };
	//�����蔻�� ��
	Sphere sphere;
	//�����蔻�� ����
	Plane plane;
	//�����蔻�� �O�p�`
	Triangle triangle;
	//�����蔻�� ���C
	Ray ray;
	Texture* titleTexture = nullptr;
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
	//�Ղꂢ��[�֌W
	float PI = 3.14f;
	float Playerradius = 0.0f;
	float PlayerSpeed = 0.0f;
	float Playerscale = 10.0f;// LaneNum�ƈꏏ�ɕς��邱��
	float PlayerCircleX = 0.0f;
	float PlayerCircleZ = 0.0f;
	float initScale = 0.0f;
	float initSpeed = 0.0f;
	float AttackSpeed = 0.0f;
	float PlayerMoveSpeed = 0.3f;
	int moveNumver = 0;
	int PlayerHP = 10;
	float angleX = 0.0f;
	float angleZ = 0.0f;
	double	angleR = 0;
	bool PlayerAttackFlag = false;
	//�Ղꂢ��[�̘r
	float frame = 0.0f;
	float frameMax = 27.0f;
	float frame2 = 0.0f;
	float frameMax2 = 80.0f;
	float frame3 = 0.0f;
	float frameMax3 = 80.0f;
	float Armradius = 0.0f;
	float ArmSpeed = 90.0f;
	float Armscale = 1.0f;
	float ArmCircleX = 0.0f;
	float ArmCircleZ = 0.0f;
	float Savescale = 0.0f;
	bool fantasyFlag = false;
	//�G�֌W
	float Enemyradius[Max] = { 0.0f };
	float EnemySpeed[Max] = { 0.0f };
	float Enemyscale[Max] = { 0.0f };// LaneNum�ƈꏏ�ɕς��邱��
	float EnemyCircleX[Max] = { 0.0f };
	float EnemyCircleZ[Max] = { 0.0f };
	int EnemyAlive[Max] = { 0 };
	int EnemyTimer[Max] = { 100 };
	int EnemyMove[Max] = { 0 };
	float EnemyangleX[Max] = { 0.0f };
	float EnemyangleZ[Max] = { 0.0f };
	double	EnemyangleR[Max] = { 0 };
	int CircleInFlag[Max] = { 0 };
	bool EnemyCatch[Max] = { false };
	
	const int PlayerMax = 5;
	Enemy* enemy[Max];

	int control = 0;
	int BossHP = 50;
	float EnemyWeight = 0.0f;
	bool ButtunFlag = false;
	int ArmMoveNumber = 0;
	int AttackMoveNumber = 0;
	int atframe = 0;
	bool BossHit = false;
	int HitTimer = 10;
	//�v���g�^�C�v�Ńo�b�N�p
	bool SpeedWeight = false;
	bool AttackWeight = false;
};