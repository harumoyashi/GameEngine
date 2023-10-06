#include "SphereCollider.h"

void SphereCollider::Update(NObj3d* obj)
{
	//オブジェクト情報をもとにコライダーの座標も移動
	centerPos_ = obj->GetMatWorld().GetWorldPos() + offset_;
}