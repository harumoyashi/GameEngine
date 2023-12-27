#pragma once
#include "IModel.h"

// オブジェクトのモデル(FBXと区別する用) //

struct ObjModel final
	: public IModel
{
	//コンストラクタ
	ObjModel()
	{
		format = ModelFormat::Obj;
	}
};