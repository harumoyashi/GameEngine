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

void Item::Generate(const NVec3& pos)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("cube");
	obj_->SetTexture("item");
	obj_->position_ = pos;
	obj_->rotation_.x = 45.f;
	obj_->rotation_.z = 45.f;
	obj_->scale_ = Player::GetInstance()->GetScale() * 2.0f;

	//コライダー設定
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x * 1.5f);	//ちょい大きめに判定取る
	collider_.SetColID("item");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Item::OnCollision, this));
}

void Item::Update()
{
	//経過時間を適用
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//くるくるまわす
	obj_->rotation_.y += 5.f * elapseSpeed_;

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
		Player::GetInstance()->LevelUp(bulletType_);
	}
}
