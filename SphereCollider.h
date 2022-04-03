#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

//���Փ˔���I�u�W�F�N�g
class SphereCollider : public BaseCollider, public Sphere
{
private:
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius) {
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	//�X�V
	void Update() override;

	inline void SetRadius(float radius) { this->radius = radius; }

private:
	//���S����̃I�t�Z�b�g
	XMVECTOR offset;
	//���a
	float radius;

};