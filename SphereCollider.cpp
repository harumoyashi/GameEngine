#include "SphereCollider.h"

void SphereCollider::Update(NObj3d* obj)
{
	//�I�u�W�F�N�g�������ƂɃR���C�_�[�̍��W���ړ�
	centerPos_ = obj->GetMatWorld().GetWorldPos() + offset_;
}