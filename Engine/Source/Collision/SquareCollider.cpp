#include "SquareCollider.h"

void SquareCollider::Update(NObj3d* obj)
{
	//オブジェクト情報をもとにコライダーの座標も移動
	centerPos_ = NVec2(obj->GetMatWorld().GetWorldPos().x, obj->GetMatWorld().GetWorldPos().z) + offset_;
}
