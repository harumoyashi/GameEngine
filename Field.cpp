#include "Field.h"

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Init()
{
	obj_ = std::make_unique<NObj3d>();
	obj_->Init();
	obj_->SetModel("cube");
	obj_->color_.SetColor255(5,5,5,255);
	obj_->scale_ = { 100.0f,0.1f,5000.0f };
	obj_->position_ = { 0,1.0f,obj_->scale_.z - 100.0f };
}

void Field::Update()
{
	obj_->Update();
}

void Field::Draw()
{
	obj_->Draw();
}
