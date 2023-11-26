#include "Boss.h"
#include "Player.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "ItemManager.h"
#include "Score.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

Boss::Boss() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), isAlive_(true), homingPower_(0.2f),
	elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_), score_(300)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("catWalk");

	//パーティクルエミッターをマネージャーに登録
	deadParticle_.SetShapeType((uint32_t)ShapeType::Cube);
	NParticleManager::GetInstance()->AddEmitter(&deadParticle_, "bossDead");
	deadParticle_.SetIsRotation(true);

	isAlive_ = false;
}

Boss::~Boss()
{
	//コライダーマネージャーから削除
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

Boss* Boss::GetInstance()
{
	static Boss instance;
	return &instance;
}

void Boss::Generate(const NVec3& pos)
{
	obj_->position_ = pos;
	oriScale_ = Player::GetInstance()->GetScale() * 3.f;
	obj_->scale_ = oriScale_;
	obj_->color_ = NColor::kRed;
	obj_->Update();

	//コライダー設定
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("boss");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Boss::OnCollision, this));

	isAlive_ = true;
	moveSpeed_ = Player::GetInstance()->GetMoveSpeed() * 1.2f;
}

void Boss::Init()
{
	isAlive_ = false;
}

void Boss::Update()
{
	if (isAlive_)
	{
		//経過時間を適用
		SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

		//移動
		Move();

		obj_->Update();
		collider_.Update(obj_.get());
	}

	//OnCollision()で呼ぶと、そのフレームでの総当たりに影響が出るからここで消してる
	if (isAlive_ == false)
	{
		//コライダーマネージャーから削除
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void Boss::Draw()
{
	if (isAlive_)
	{
		obj_->SetBlendMode(BlendMode::None);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::None);
	}
}

void Boss::OnCollision()
{
	//当たった相手が弾だった時の処理
	if (collider_.GetColInfo()->GetColID() == "bullet")
	{
		hp_--;

		//弾当たった時の音鳴らす
		//NAudioManager::GetInstance()->Play("vanishSE");
	}
}

void Boss::DeadParticle()
{
	if (isAlive_)
	{
		deadParticle_.SetPos(obj_->position_);
		deadParticle_.Add(
			100, 1.5f, obj_->color_, 0.1f, 1.0f, { -1.5f,-1.5f,-1.5f }, { 1.5f,1.5f,1.5f }, 0.03f, -NVec3::one, NVec3::one);
	}
}

void Boss::Move()
{
	NVec3 p2bVec = obj_->position_ - Player::GetInstance()->GetPos();
	float isLR = moveVelo_.Cross({ p2bVec.x,p2bVec.z });

	moveAngle_ += isLR * homingPower_;

	if (abs(moveAngle_) > PI2)
	{
		moveAngle_ = 0.f;
	}

	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	//回転
	obj_->rotation_.y = moveAngle_ * 360 / PI2;

#ifdef _DEBUG
	ImGui::Begin("BossParameter");
	ImGui::Text("moveAngle:%f", moveAngle_);
	ImGui::Text("p2bVec:%f", p2bVec);
	ImGui::Text("isLR:%f", isLR);
	ImGui::End();
#endif //DEBUG
}
