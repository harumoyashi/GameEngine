#include "NMaterial.h"


NMaterial::NMaterial()
{
	//�Ƃ肠�����`�悳���悤�ɐݒ肵�Ƃ�
	ambient = { 0.3f,0.3f,0.3f };
	diffuse = { 0.0f,0.0f,0.0f };
	specular = { 0.0f,0.0f,0.0f };
	alpha = 1.0f;
}

NMaterial::~NMaterial()
{
}