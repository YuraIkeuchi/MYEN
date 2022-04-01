#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	//���[���h�s�񂩂���W��E�o
	const XMMATRIX& matWorld = object3d->GetMatWorld();

	//�e�̃����o�ϐ����X�V
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}