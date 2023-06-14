#include "NMaterial.h"


NMaterial::NMaterial()
{
	//Ç∆ÇËÇ†Ç¶Ç∏ï`âÊÇ≥ÇÍÇÈÇÊÇ§Ç…ê›íËÇµÇ∆Ç≠
	ambient = { 0.3f,0.3f,0.3f };
	diffuse = { 0.0f,0.0f,0.0f };
	specular = { 0.0f,0.0f,0.0f };
	alpha = 1.0f;
}

NMaterial::~NMaterial()
{
}