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

	//現在のシーンがどこか判断する変数(変数自体は個々のシーン内で書き換える)
	std::string sceneID = "";
};