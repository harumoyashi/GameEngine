#include "Item.h"
#include "Player.h"
#include "NCollisionManager.h"

Item::Item() :
	moveVelo_({ 0,0 }), moveSpeed_(0.2f), isAlive_(true), elapseSpeed_(0.0f)
{
}

Item::~Item()
{
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

void Item::Generate(const NVec3& pos, const float moveAngle)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("cube");
	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale();
	obj_->color_ = NColor::kYellow;

	//コライダー設定
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("item");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Item::OnCollision, this));
}

void Item::Update()
{
	//経過時間を適用
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()で呼ぶと、そのフレームでの総当たりに影響が出るからここで消してる
	if (isAlive_ == false)
	{
		//コライダーマネージャーから削除
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void Item::Draw()
{
	if (isAlive_)
	{
		obj_->Draw();
	}
}

void Item::OnCollision()
{
	//当たった相手がプレイヤーだった時の処理
	if (collider_.GetColInfo()->GetColID() == "player")
	{
		isAlive_ = false;
		Player::GetInstance()->LevelUp(BulletType::LineBullet);
	}
}
