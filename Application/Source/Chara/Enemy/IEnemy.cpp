#include "IEnemy.h"
#include "Player.h"
#include "Field.h"
#include "NCameraManager.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "ItemManager.h"
#include "Score.h"

//スピードは基本プレイヤーよりちょい遅め
IEnemy::IEnemy() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.04f), isAlive_(true),
	elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_), score_(10), isItem_(false), isDraw_(true)
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
	oriScale_ = Player::GetInstance()->GetScale();
	obj_->scale_ = oriScale_;
	obj_->color_ = NColor::kLightblue;
	obj_->Update();

	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x * 2.f);
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

	//リズム乗らせるからスケールを常に更新
	obj_->scale_ = oriScale_ + addScale_;

	//移動
	Move();

	//デバッグカメラの時はカメラ基準で消えられると困るから消さない
	if (NCameraManager::GetInstance()->GetIsDebugCamera() == false)
	{
		// 「2Dになおした行動範囲+画面端座標」を「2Dに直した敵の座標+敵の半径」が超えた場合殺す //
		IsInActiveArea();
	}

	//画面内にいる奴だけ描画フラグ立てる
	isDraw_ = IsInScreen();

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
	if (isAlive_ && isDraw_)
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
			uint32_t bulType = MathUtil::Random(0, (uint32_t)BulletType::MaxType);
			ItemManager::GetInstance()->Generate(obj_->position_, (BulletType)bulType);
		}
		NAudioManager::GetInstance()->Play("vanishSE");
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

bool IEnemy::IsInScreen()
{
	// 「画面端座標」を「2Dに直した敵の座標+敵の半径」が超えたか判定 //
	//行動範囲をスクリーン座標に変換してウィンドウ座標と足す
	NMatrix4 matWorldRight, matWorldLeft;

	NVec2 objPosPlusScale;	//オブジェのスケール足した場合の2D座標
	NVec2 objPosMinusScale;	//オブジェのスケール引いた場合の2D座標
	//スケール足したのと引いたのそれぞれ平行移動行列として求める
	NMatrix4 posPlusScaleMat =
		NMatrix4::Translation(obj_->GetMatWorld().GetWorldPos() + obj_->GetMatWorld().GetScale());
	NMatrix4 posMinusScaleMat =
		NMatrix4::Translation(obj_->GetMatWorld().GetWorldPos() - obj_->GetMatWorld().GetScale());
	//スクリーン座標に直して
	objPosPlusScale = MathUtil::WorldToScreen(posPlusScaleMat);
	objPosMinusScale = MathUtil::WorldToScreen(posMinusScaleMat);
	//画面内か判定
	/*if (0 < objPosPlusScale.x && NWindows::kWin_width > objPosMinusScale.x &&
		0 < objPosMinusScale.y && NWindows::kWin_height > objPosPlusScale.y)
	{
		return true;
	}
	else
	{
		return false;
	}*/
	//判定わかりやすくする用
	if (0 < objPosMinusScale.x && NWindows::kWin_width > objPosPlusScale.x &&
		0 < objPosPlusScale.y && NWindows::kWin_height > objPosMinusScale.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void IEnemy::IsInActiveArea()
{
	// 「2Dになおした行動範囲+画面端座標」を「2Dに直した敵の座標+敵の半径」が超えてるか判定 //
	float borderLineRight, borderLineLeft;	//超えたら死ぬとこ

	//「2Dに直した行動範囲+画面端座標」
	//行動範囲をスクリーン座標に変換してウィンドウ座標と足す
	NMatrix4 matWorldRight, matWorldLeft;

	//行動範囲座標を平行移動行列に変換
	matWorldRight = matWorldRight.Translation(NVec3(Field::GetInstance()->GetActivityAreaX(), 0, 0));
	matWorldLeft = matWorldLeft.Translation(NVec3(-Field::GetInstance()->GetActivityAreaX(), 0, 0));
	//行動範囲座標をスクリーン座標に変換して、画面サイズ分ずらしたのが敵の行動範囲
	borderLineRight = MathUtil::WorldToScreen(matWorldRight).x + NWindows::kWin_width * 0.5f;
	borderLineLeft = MathUtil::WorldToScreen(matWorldLeft).x - NWindows::kWin_width * 0.5f;

	float objRight, objLeft;	//オブジェの右端左端
	//スケール足したのと引いたのそれぞれ平行移動行列として求める
	NMatrix4 objRightMat =
		NMatrix4::Translation(obj_->GetMatWorld().GetWorldPos() + obj_->GetMatWorld().GetScale());
	NMatrix4 objLeftMat =
		NMatrix4::Translation(obj_->GetMatWorld().GetWorldPos() - obj_->GetMatWorld().GetScale());
	//スクリーン座標に直して
	objRight = MathUtil::WorldToScreen(objRightMat).x;
	objLeft = MathUtil::WorldToScreen(objLeftMat).x;
	//ボーダーライン超えてるか判定
	if (borderLineRight < objLeft || borderLineLeft > objRight)
	{
		isAlive_ = false;
	}
}

void IEnemy::Move()
{
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	//回転
	obj_->rotation_.y = moveAngle_ * 360 / PI2;
}

void IEnemy::SetisItem(bool isItem)
{
	isItem_ = isItem;
	obj_->color_ = NColor::kYellow;	//アイテム持ってる敵は黄色にする
}
