#pragma once
#include "NObj3d.h"

//床。タイリングと行動範囲以外色変えする専用のシェーダーがいる。
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