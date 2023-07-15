#include "Player.h"
#include "NInput.h"
#include "BulletFactory.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

Player::Player()
{
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("cat");

	//パーティクルエミッターをマネージャーに登録
	NParticleManager::GetInstance()->AddEmitter(&deadParticle_,"playerDead");
}

Player::~Player()
{
	//コライダーマネージャーから削除
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	//パーティクルマネージャーから削除
	NParticleManager::GetInstance()->EraseEmitter("playerDead");
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

bool Player::Init()
{
	obj_->position_ = {};
	obj_->scale_ = 0.1f;
	obj_->color_.SetColor255(240, 30, 20, 255);	//オレンジっぽく
	obj_->Update();

	isAlive_ = true;
	isGodmode_ = false;
	isDraw_ = true;

	moveSpeed_ = 0.05f;

	godmodeTimer_.Reset();
	godmodeTimer_.SetMaxTimer(120.0f);

	isMove_ = false;

	//弾のレベルたち
	lineLevel_ = 1;
	sideLevel_ = 1;
	wideLevel_ = 0;
	roketLevel_ = 0;

	//コライダー設定
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("player");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Player::OnCollision, this));

	return true;
}

void Player::Update()
{
	if (isAlive_)
	{
		Move();
		Shot();
	}
	else
	{
		elapseSpeed_ = 0.01f;	//死んだらスローモーションに
	}

	obj_->Update();
	collider_.Update(obj_.get());
}

void Player::Draw()
{
	if (isAlive_)	//ほんとは違うけど一旦わかりやすいので
	{
		if (isDraw_)
		{
			obj_->Draw();
		}
	}
}

void Player::Move()
{
	isMove_ = false;

	//パッド接続されてるなら
	if (NInput::GetIsConnect())
	{
		//スティック移動
		moveVelo_ = NInput::GetStick();
		//イージング的な感じにして速度の可変明確に
		moveVelo_.x *= moveVelo_.x * NInput::GetStick().x;
		moveVelo_.y *= moveVelo_.y * NInput::GetStick().y;
	}
	else
	{
		moveVelo_ = { 0,0 };
		//いずれかのキーを押したとき
		if (NInput::IsKey(DIK_W) || NInput::IsKey(DIK_S) || NInput::IsKey(DIK_D) || NInput::IsKey(DIK_A))
		{
			if (NInput::IsKey(DIK_W)) { moveVelo_.y = +1.0f; }
			else if (NInput::IsKey(DIK_S)) { moveVelo_.y = -1.0f; }
			if (NInput::IsKey(DIK_D)) { moveVelo_.x = +1.0f; }
			else if (NInput::IsKey(DIK_A)) { moveVelo_.x = -1.0f; }
		}
	}

	if (moveVelo_.Length() > 0.0f)	//入力されてたら
	{
		isMove_ = true;
	}

	elapseSpeed_ = moveVelo_.Length();	//移動量によって経過時間変化

	//移動量を加算
	obj_->position_.x += moveVelo_.x * moveSpeed_;
	obj_->position_.z += moveVelo_.y * moveSpeed_;

	//移動方向に合わせて回転
	if (isMove_)			//入力されてたら
	{
		NVector2 velo = moveVelo_;	//moveVelo_の値が変わらないように格納
		velo.Normalize();
		moveAngle_ = MathUtil::Radian2Degree(acosf(velo.Dot({ 0,1 })));
		if (velo.x < 0)
		{
			moveAngle_ = -moveAngle_;
		}

		obj_->rotation_.y = moveAngle_;
	}

	//リリースでもいじりたいからifdefで囲ってない
	static int lineLv = 1, sideLv = 1;
	ImGui::Begin("PlayerParameter");
	ImGui::SliderFloat("MoveSpeed", &moveSpeed_, 0.01f, 1.0f);
	ImGui::SliderInt("LineLevel", &lineLv, 0, 5);
	ImGui::SliderInt("SideLevel", &sideLv, 0, 5);
	ImGui::End();
	lineLevel_ = lineLv;
	sideLevel_ = sideLv;
}

void Player::Shot()
{
	BulletFactory::GetInstance()->Update();

	if (lineLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(IBullet::BulletType::LineBullet,
				obj_->position_ + NVector3(0, obj_->scale_.y, 0), lineLevel_);
	}

	if (sideLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(IBullet::BulletType::SideBullet,
				obj_->position_ + NVector3(0, obj_->scale_.y, 0), sideLevel_);
	}

	/*if (wideLevel)
	{

	}

	if (roketLevel)
	{

	}*/
}

void Player::OnCollision()
{
	//当たった相手が敵だった時の処理
	if (collider_.GetColInfo()->GetColID() == "enemy")
	{
		DeadParticle();
		isAlive_ = false;
	}
}

void Player::DeadParticle()
{
	if (isAlive_)
	{
		NParticleManager::GetInstance()->emitters_["playerDead"]->SetIsRotation(true);
		NParticleManager::GetInstance()->emitters_["playerDead"]->SetPos(GetPos());
		NParticleManager::GetInstance()->emitters_["playerDead"]->Add(
			150, 100, obj_->color_, 0.1f, 1.0f, { -1,-1,-1 }, { 1,1,1 }, { 0,0,0 }, { -1,-1,-1 }, { 1,1,1 });
	}
}
