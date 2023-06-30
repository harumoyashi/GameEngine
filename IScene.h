#pragma once
#include <string>

class IScene
{
public:
	IScene() {};
	virtual ~IScene() = default;

	virtual void LoadResources() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DrawBackSprite() = 0;
	virtual void Draw3D() = 0;
	virtual void DrawForeSprite() = 0;

	//���݂̃V�[�����ǂ������f����ϐ�(�ϐ����̂͌X�̃V�[�����ŏ���������)
	std::string sceneID = "";
};