#pragma once
#include "NObj3d.h"

//���B�^�C�����O�ƍs���͈͈ȊO�F�ς������p�̃V�F�[�_�[������B
class Field
{
private:
	std::unique_ptr<NObj3d> obj_;

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void Draw();
};