#pragma once

#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:
	//�ÓI�����o�ϐ�
	static CollisionManager* GetInstance();

public://�����o�֐�
	//�R���C�_�[�̒ǉ�
	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	//�R���C�_�[�̍폜
	inline void RemoveCollider(BaseCollider* collider) {
		colliders.remove(collider);
	}

	//���ׂĂ̏Փ˂Ă�����
	void CheckAllCollisions();

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;

	CollisionManager& operator=(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders;
};