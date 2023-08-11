#pragma once
#include "IModel.h"

struct ObjModel final
	: public IModel
{
	ObjModel()
	{
		format = ModelFormat::Obj;
	}
};