#include "NMaterial.h"


NMaterial::NMaterial()
{
	//とりあえず描画されるように設定しとく
	ambient = { 0.3f,0.3f,0.3f };
	diffuse = { 0.0f,0.0f,0.0f };
	specular = { 0.0f,0.0f,0.0f };
	alpha = 255.0f;
	color = { 255.0f,255.0f,255.0f };
	
	Init();
}

NMaterial::~NMaterial()
{
}

void NMaterial::Init()
{
	cbColor.Init();
	cbMaterial.Init();
	TransferColor();
	TransferMaterial();
}

void NMaterial::TransferColor()
{
	//0.0f~1.0fに直す
	color.r /= 255.0f;
	color.g /= 255.0f;
	color.b /= 255.0f;
	color.a /= 255.0f;
	//値を書き込むと自動的に転送される
	cbColor.constMap->color = color;
}

void NMaterial::TransferMaterial()
{
	cbMaterial.constMap->ambient = ambient;
	cbMaterial.constMap->diffuse = diffuse;
	cbMaterial.constMap->specular = specular;
}

void NMaterial::SetColor(int R, int G, int B, int A)
{
	color.r = static_cast<float>(R);
	color.g = static_cast<float>(G);
	color.b = static_cast<float>(B);
	color.a = static_cast<float>(A);
	TransferColor();
}

void NMaterial::SetCBV()
{
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial.constBuff->GetGPUVirtualAddress());
}
