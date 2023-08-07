#pragma once
#include "IModel.h"

struct NObjModel final
	: public IModel
{
	NObjModel()
	{
		format = ModelFormat::Obj;
	}
};