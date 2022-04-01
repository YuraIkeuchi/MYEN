#pragma once
#include"CollisionTypes.h"
#include"Object3d.h"
#include "CollisionInfo.h"


class BaseCollider {
public:
	BaseCollider() = default;


	inline void SetObject(Object3d* object) {
		this->object3d = object;
	}

	inline Object3d* GetObject3d() {
		return object3d;
	}
	///<summary>
	///衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}
	///<summary>
	///更新
	///</summary>
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }
protected:
	Object3d* object3d = nullptr;
	CollisionShapeType shapeType = SHAPE_UNKNOWN;

};

