#include "CircleCollider.h"

void CircleCollider::Update(NObj3d* obj)
{
	if (isActive_)
	{
		//オブジェクト情報をもとにコライダーの座標も移動
		centerPos_ = NVec2(obj->GetMatWorld().GetWorldPos().x, obj->GetMatWorld().GetWorldPos().z) + offset_;
		radius_ = obj->GetMatWorld().GetScale().x;
	}
}
