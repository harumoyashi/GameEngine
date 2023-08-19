#pragma once
#include "EnemyManager.h"
#include "Mouse.h"

class EnemyFactory final
{
private:
	Mouse mouse;

	NVec3 offset;			//�G�~�b�^�[���łǂꂾ���o���ʒu���炷��
	bool isCollision = false;	//�G���m������Ă邩�t���O

	uint32_t mouseSideNum = 10;				//�T�Q(��)�̏o����
	NVec2 mouseSideEmitter = {2.0f,1.0f};	//�T�Q(��)�̏o���͈�

public:
	static EnemyFactory* GetInstance();

	void Update();
	//�G�̐���
	//type:�G�̎��
	//pos:�����̊���W
	//isItem:�A�C�e�����������G�𐶐����邩
	void Create(IEnemy::EnemyType type, NVec3 pos,bool isItem);

private:
	EnemyFactory() = default;
	EnemyFactory(const EnemyFactory&) = delete;
	~EnemyFactory() = default;
	EnemyFactory& operator=(const EnemyFactory&) = delete;
};