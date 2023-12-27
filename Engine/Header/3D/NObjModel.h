#pragma once
#include "IModel.h"

// オブジェクトのモデル(FBXと区別する用) //

struct ObjModel final
	: public IModel
{
	ObjModel()
	{
		format = ModelFormat::Obj;
	}
};