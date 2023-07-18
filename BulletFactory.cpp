#include "BulletFactory.h"
#include "NAudioManager.h"

BulletFactory* BulletFactory::GetInstance()
{
	static BulletFactory instance;
	return &instance;
}

void BulletFactory::Update()
{
	line_.LineUpdate();
	side_.SideUpdate();
}

void BulletFactory::Create(IBullet::BulletType type, NVector3 pos, uint32_t level)
{
	switch (type)
	{
	case IBullet::BulletType::LineBullet:
		if (line_.GetIsCanShot())	//���Ă�Ȃ�
		{
			//�Ƃ肠�����ǉ�
			BulletManager::GetInstance()->bullets_.emplace_back();
			//���x���i�[����(���x���p�̏������Ή�)
			line_.SetLevel(level);
			//�Ή�������ނɏ��L���������Đ���
			BulletManager::GetInstance()->bullets_.back() = std::make_unique<LineBullet>();
			BulletManager::GetInstance()->bullets_.back()->Generate(pos);
			//�������I�������^�C�}�[�ƃt���O�����Z�b�g
			line_.SetIsCanShot(false);
			line_.ReSetShotCoolTimer();

			NAudioManager::Play("shotSE",false,0.5f);
		}

		break;
	case IBullet::BulletType::SideBullet:
		if (side_.GetIsCanShot())	//���Ă�Ȃ�
		{
			//2������(��Ń��x���ɑΉ�����for������ɉ�)
			for (size_t i = 0; i < side_.GetAllAtOnceNum(); i++)
			{
				//�Ƃ肠�����ǉ�
				BulletManager::GetInstance()->bullets_.emplace_back();
				//���x���i�[����(���x���p�̏������Ή�)
				side_.SetLevel(level);
				//�Ή�������ނɏ��L���������Đ���
				BulletManager::GetInstance()->bullets_.back() = std::make_unique<SideBullet>();
				//���E�ɏo���悤��
				BulletManager::GetInstance()->bullets_.back()->Generate(pos, MathUtil::Degree2Radian(90.0f + 180.0f * i));
			}
			//�������I�������^�C�}�[�ƃt���O�����Z�b�g
			side_.SetIsCanShot(false);
			side_.ReSetShotCoolTimer();
		}

		break;
	default:
		break;
	}
}