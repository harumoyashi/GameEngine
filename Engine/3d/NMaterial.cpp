#include "NMaterial.h"


NMaterial::NMaterial()
{
	//とりあえず描画されるように設定しとく
	ambient = { 0.3f,0.3f,0.3f };
	diffuse = { 0.0f,0.0f,0.0f };
	specular = { 0.0f,0.0f,0.0f };
	alpha = 255.0f;
}

NMaterial::~NMaterial()
{
}