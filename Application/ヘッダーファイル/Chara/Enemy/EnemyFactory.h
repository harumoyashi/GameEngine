#pragma once
#include "EnemyManager.h"
#include "Mouse.h"

class EnemyFactory final
{
private:
	Mouse mouse;

	NVector3 offset;			//�G�~�b�^�[���łǂꂾ���o���ʒu���炷��
	bool isCollision = false;	//�G���m������Ă邩�t���O

	uint32_t mouseSideNum = 10;				//�T�Q(��)�̏o����
	NVector2 mouseSideEmitter = {2.0f,1.0f};	//�T�Q(��)�̏o���͈�

public:
	static EnemyFactory* GetInstance();

	void Update();
	//�G�̐���
	void Create(IEnemy::EnemyType type, NVector3 pos);

private:
	EnemyFactory() = default;
	EnemyFactory(const EnemyFactory&) = delete;
	~EnemyFactory() = default;
	EnemyFactory& operator=(const EnemyFactory&) = delete;
};