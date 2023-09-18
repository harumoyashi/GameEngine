#include "IEnemy.h"
#include "Player.h"
#include "Field.h"
#include "NCamera.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "ItemManager.h"
#include "Score.h"

//スピードは基本プレイヤーよりちょい遅め
IEnemy::IEnemy() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.04f), isAlive_(true),
	elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_),score_(10), isItem_(false)
{
}

IEnemy::~IEnemy()
{
}

void IEnemy::Generate(const NVec3& pos, const float moveAngle, const std::string& modelname)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel(modelname);

	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale() * 1.5f;
	obj_->color_ = NColor::kLightblue;
	obj_->Update();

	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("enemy");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&IEnemy::OnCollision, this));

	moveAngle_ = moveAngle;

	NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->SetIsRotation(true);
}

void IEnemy::Init()
{
}

void IEnemy::Update()
{
	//経過時間を適用
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//移動
	Move();

	// 「2Dに直した行動範囲+画面端座標」を「2Dに直した敵の座標+敵の半径」が超えた場合殺す //
	float borderLineRight, borderLineLeft;	//超えたら死ぬとこ

	//「2Dに直した行動範囲+画面端座標」
	//行動範囲をスクリーン座標に変換してウィンドウ座標と足す
	NMatrix4 matWorldRight, matWorldLeft;

	matWorldRight = matWorldRight.Translation(NVec3(Field::GetInstance()->GetActivityAreaX(), 0, 0));
	borderLineRight = MathUtil::WorldToScreen(NVec3(), matWorldRight).x + NWindows::kWin_width * 0.5f;

	matWorldLeft = matWorldLeft.Translation(NVec3(-Field::GetInstance()->GetActivityAreaX(), 0, 0));
	borderLineLeft = MathUtil::WorldToScreen(NVec3(), matWorldLeft).x - NWindows::kWin_width * 0.5f;

	float objRight, objLeft;	//オブジェの右端左端
	objRight = MathUtil::WorldToScreen(obj_->position_ + obj_->scale_, obj_->GetMatWorld()).x;
	objLeft = MathUtil::WorldToScreen(obj_->position_ - obj_->scale_, obj_->GetMatWorld()).x;
	//ボーダーライン超えたら殺す
	if (borderLineRight < objLeft || borderLineLeft > objRight)
	{
		isAlive_ = false;
	}

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()で呼ぶと、そのフレームでの総当たりに影響が出るからここで消してる
	if (isAlive_ == false)
	{
		//コライダーマネージャーから削除
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void IEnemy::Draw()
{
	if (isAlive_)
	{
		obj_->SetBlendMode(BlendMode::None);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::None);
	}
}

void IEnemy::OnCollision()
{
	//当たった相手が弾だった時の処理
	if (collider_.GetColInfo()->GetColID() == "bullet")
	{
		DeadParticle();
		isAlive_ = false;
		Score::AddScore(score_);
		//アイテム持ってるフラグ立ってたらアイテム落とす
		if (isItem_)
		{
			uint32_t bulType = MathUtil::Random(0,(uint32_t)BulletType::MaxType);
			ItemManager::GetInstance()->Generate(obj_->position_, (BulletType)bulType);
		}
		NAudioManager::Play("vanishSE");
	}
}

void IEnemy::AddEmitter(uint32_t eneNum)
{
	//識別番号をつける
	enemyNum_ = eneNum;
	//パーティクルエミッターをマネージャーに登録
	NParticleManager::GetInstance()->enemyEmitters_.emplace_back();
	NParticleManager::GetInstance()->enemyEmitters_.back() = &deadParticle_;

}

void IEnemy::DeadParticle()
{
	if (isAlive_)
	{
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->SetPos(GetPos());
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->Add(
			15, 0.25f, obj_->color_, 0.1f, 0.4f, { -0.5f,-0.1f,-0.5f }, { 0.5f,0.1f,0.5f }, NVec3::zero, -NVec3::one, NVec3::one);
	}
}

void IEnemy::Move()
{
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	//向かってる方とは逆っぽい
	obj_->rotation_.y = -MathUtil::Radian2Degree(moveAngle_);
}

void IEnemy::SetisItem(bool isItem)
{
	isItem_ = isItem;
	obj_->color_ = NColor::kYellow;	//アイテム持ってる敵は黄色にする
}
