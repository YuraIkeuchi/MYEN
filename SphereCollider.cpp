#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	//ワールド行列から座標を摘出
	const XMMATRIX& matWorld = object3d->GetMatWorld();

	//弾のメンバ変数を更新
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}