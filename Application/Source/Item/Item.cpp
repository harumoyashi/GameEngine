#include "Item.h"
#include "Player.h"
#include "NCollisionManager.h"
#include "NAudioManager.h"

Item::Item() :
	moveVelo_({ 0,0 }), moveSpeed_(0.2f), isAlive_(true), elapseSpeed_(0.0f)
{
}

Item::~Item()
{
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

void Item::Generate(const NVec3& pos, bool isPop)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("cube");
	obj_->SetTexture("item");
	obj_->position_ = pos;
	obj_->rotation_.x = 45.f;
	obj_->rotation_.z = 45.f;
	obj_->scale_ = Player::GetInstance()->GetScale() * 2.5f;

	//コライダー設定
	collider_.SetCenterPos({ obj_->position_.x,obj_->position_.z });
	collider_.SetRadius(obj_->scale_.x * 2.5f);	//ちょい大きめに判定取る
	collider_.SetColID("item");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Item::OnCollision, this));

	if (isPop)
	{
		moveVelo_ = { 0.f,0.8f };
	}
	else
	{
		moveVelo_ = { 0,0 };
	}
}

void Item::Update()
{
	//経過時間を適用
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//くるくるまわす
	obj_->rotation_.y += 5.f * elapseSpeed_;

	//動かす
	obj_->position_.y += moveVelo_.y * elapseSpeed_;
	//重力加算
	moveVelo_.y -= 0.1f * elapseSpeed_;
	//行きすぎないように
	obj_->position_.y = MathUtil::Clamp(obj_->position_.y, collider_.GetRadius(), 100.f);

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
		obj_->SetBlendMode(BlendMode::Alpha);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::Alpha);
	}
}

void Item::OnCollision()
{
	//当たった相手がプレイヤーだった時の処理
	if (collider_.GetColInfo()->GetColID() == "player")
	{
		isAlive_ = false;
		Player::GetInstance()->LevelUp(bulletType_);
		//無敵アイテムの時だけ別のSE鳴らす
		if (bulletType_ != BulletType::MaxType)
		{
			NAudioManager::GetInstance()->Play("itemGetSE");
		}
		else
		{
			NAudioManager::GetInstance()->Play("mutekiGetSE");
		}
	}
}
