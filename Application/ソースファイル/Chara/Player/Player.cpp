#include "Player.h"
#include "NInput.h"
#include "BulletFactory.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "NCameraManager.h"
#include "Field.h"
#include "RadialBlur.h"
#include "Bloom.h"
#include "UI.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

Player::Player()
{
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("catWalk");

	//パーティクルエミッターをマネージャーに登録
	NParticleManager::GetInstance()->AddEmitter(&deadParticle_, "playerDead");
	deadParticle_.SetIsRotation(true);

	NParticleManager::GetInstance()->AddEmitter(&clearParticle_, "gameClear");
	clearParticle_.SetIsRotation(true);
}

Player::~Player()
{
	//コライダーマネージャーから削除
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
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
	obj_->color_ = NColor::kPlayer;	//オレンジっぽく
	obj_->SetIsElapseAnime(false);	//経過時間無視しておく
	obj_->Update();

	isAlive_ = true;
	isGodmode_ = false;
	isDraw_ = true;

	moveSpeed_ = 0.05f;

	godmodeTimer_.Reset();
	godmodeTimer_.maxTime_ = 2.5f;

	isMove_ = true;
	elapseSpeed_ = 0.0f;

	//弾のレベルたち
	lineLevel_ = 1;
	sideLevel_ = 0;
	wideLevel_ = 0;
	roketLevel_ = 0;

	//コライダー設定
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("player");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Player::OnCollision, this));

	deadEffectTimer_ = 1.5f;	//スローは考慮せずに何秒か
	deadEffectTimer_.Reset();

	faildEffectTimer_ = 3.0f;
	faildEffectTimer_.Reset();

	clearParticleTimer_.Reset();

	return true;
}

void Player::Update()
{
	//タイマー更新
	deadEffectTimer_.Update();

	if (isAlive_)
	{
		//カメラ遷移が終わってたら動き出しておｋ
		if (NCameraManager::GetInstance()->GetIsNormalCameraChanged())
		{
			Move();
		}
		if (isMove_)	//動けないよ判定の時は弾を撃たないように
		{
			Shot();
		}
	}

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()で呼ぶと、そのフレームでの総当たりに影響が出るからここで消してる
	if (isAlive_ == false)
	{
		if (deadEffectTimer_.GetStarted() == false)
		{
			deadEffectTimer_.Start();
		}

		if (deadEffectTimer_.GetTimeRate() <= 0.2f)	//死亡演出の2割はヒットストップに使う
		{
			isFlashing_ = ((int)(deadEffectTimer_.GetTimeRate() * 100.0f) % 2 == 0);
			if (isFlashing_ ^ obj_->color_ == NColor::kWhite)
			{
				obj_->color_ = NColor::kPlayer;	//オレンジっぽく
			}
			else
			{
				obj_->color_ = NColor::kWhite;
			}
			elapseSpeed_ = 0.0f;
		}
		else
		{
			obj_->color_ = NColor::kPlayer;	//オレンジっぽく
			DeadParticle();
			elapseSpeed_ = slowElapseTime_;			//ヒットストップ終わったらスローに
			isDraw_ = false;						//通常は死んだら描画しない
		}

		//コライダーマネージャーから削除
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);

		deadPos_ = GetPos();	//リザルト用に死んだ座標を記録
	}

	//死亡時のパーティクルが出ていないのであればラジアルブラーはかけない
	if (deadParticle_.GetParticlesDead())
	{
		Bloom::Init();		//ラジアルブラー切ってブルームに戻す
	}
}

void Player::ClearUpdate()
{
	isDraw_ = true;						//絶対描画させる
	Bloom::Init();						//ラジアルブラー切ってブルームに戻す
	obj_->rotation_.y = 0.0f;			//前に向かせる
	obj_->position_.z += 0.05f;			//前に向かって走り続ける

	obj_->Update();

	//クリア時パーティクル用タイマー開始
	clearParticleTimer_.Roop();
	clearParticleTimer_.Update();

	//クリア時にクラッカーみたいなパーティクルが通り道に出るやつ
	if (clearParticleTimer_.GetTimeRate() <= 0.0f)
	{
		for (uint32_t i = 0; i < 7; i++)
		{
			clearParticle_.SetPos(obj_->position_ + NVec3(7.f, 0.f, (float)i * -7.f));
			clearParticle_.Add(
				10, 1.5f, NColor::kWhite, 0.1f, 0.8f,
				{ -0.3f,0.1f,-0.3f }, { 0.3f,0.5f,0.3f },
				NVec3::zero, -NVec3::one, NVec3::one);
		}
	}
}

void Player::FaildUpdate()
{
	//タイマーループ
	faildEffectTimer_.Roop();

	isDraw_ = true;						//絶対描画させる
	Bloom::Init();						//ラジアルブラー切ってブルームに戻す
	obj_->position_ = deadPos_ + NVec3(3.0f, 2.0f, -8.0f);	//死んだ座標を基準に適当な値足してそれっぽくする

	//その場で回転させる
	obj_->rotation_.y = MathUtil::Radian2Degree(faildEffectTimer_.GetTimeRate() * PI2);

	obj_->Update();
}

void Player::Draw()
{
	if (isDraw_)
	{
		obj_->SetBlendMode(BlendMode::None);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::None);
	}
}

void Player::Move()
{
	if (isMove_)
	{
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
#ifdef _DEBUG
			//床との当たり判定見たりするための上下移動
			if (NInput::IsKey(DIK_Q)) { obj_->position_.y += 0.1f; }
			else if (NInput::IsKey(DIK_E)) { obj_->position_.y -= 0.1f; }
#endif
		}

		elapseSpeed_ = abs(moveVelo_.x) + abs(moveVelo_.y);	//移動量によって経過時間変化
		elapseSpeed_ = MathUtil::Clamp(elapseSpeed_, 0.0f, 1.0f);

		//移動量を加算
		obj_->position_.x += moveVelo_.x * moveSpeed_;
		obj_->position_.z += moveVelo_.y * moveSpeed_;
		//加算後に行動範囲超えてる場合は超えないようにする
		obj_->position_.x = (std::max)(obj_->position_.x, -Field::GetInstance()->GetActivityAreaX() + obj_->scale_.x);
		obj_->position_.x = (std::min)(obj_->position_.x, Field::GetInstance()->GetActivityAreaX() - obj_->scale_.x);

		//移動方向に合わせて回転
		if (moveVelo_.Length() > 0.0f)			//入力されてたら
		{
			NVec2 velo = moveVelo_;	//moveVelo_の値が変わらないように格納
			velo.Normalize();
			moveAngle_ = MathUtil::Radian2Degree(acosf(velo.Dot({ 0,1 })));
			if (velo.x < 0)
			{
				moveAngle_ = -moveAngle_;
			}

			obj_->rotation_.y = moveAngle_;
		}
	}

#ifdef _DEBUG
	//弾レベルいじいじ用変数
	static bool isLevelMane = false;
	static int lineLv = 1, sideLv = 1, wideLv = 1;
	ImGui::Begin("PlayerParameter");
	ImGui::Checkbox("LevelManagement", &isLevelMane);	//弾レベル管理できるするかフラグ指定
	//弾レベルいじいじ
	if (isLevelMane)
	{
		ImGui::SliderInt("LineLevel", &lineLv, 0, maxBulLevel_);
		ImGui::SliderInt("SideLevel", &sideLv, 0, maxBulLevel_);
		ImGui::SliderInt("WideLevel", &wideLv, 0, maxBulLevel_);
		lineLevel_ = lineLv;
		sideLevel_ = sideLv;
		wideLevel_ = wideLv;
	}
	//その他のパラメータいじいじ
	ImGui::SliderFloat("MoveSpeed", &moveSpeed_, 0.01f, 1.0f);
	ImGui::Text("MoveVelo:%f,%f", moveVelo_.x, moveVelo_.y);
	ImGui::Text("MoveLen:%f", moveVelo_.Length());
	ImGui::End();
#endif //DEBUG
}

void Player::Shot()
{
	BulletFactory::GetInstance()->Update();

	if (lineLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(BulletType::LineBullet,
				obj_->position_ + NVec3(0, obj_->scale_.y, 0), lineLevel_);
	}

	if (sideLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(BulletType::SideBullet,
				obj_->position_ + NVec3(0, obj_->scale_.y, 0), sideLevel_);
	}

	if (wideLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(BulletType::WideBullet,
				obj_->position_ + NVec3(0, obj_->scale_.y, 0), wideLevel_);
	}
}

void Player::OnCollision()
{
	//当たった相手が敵だった時の処理
	if (collider_.GetColInfo()->GetColID() == "enemy")
	{
		NAudioManager::GetInstance()->Play("deadSE");
		SetIsAlive(false);
		NInput::Vibration(0.8f,0.8f,0.3f);
	}
}

void Player::DeadParticle()
{
	if (isDraw_)
	{
		NAudioManager::GetInstance()->Play("explosionSE");
		RadialBlur::Init();		//ラジアルブラーかける
		
		deadParticle_.SetPos(obj_->position_);
		deadParticle_.Add(
			100, 1.5f, obj_->color_, 0.1f, 1.0f, { -2,-2,-2 }, { 2,2,2 }, NVec3::zero, -NVec3::one, NVec3::one);
	}
}

void Player::LevelUp(BulletType bulletType)
{
	//指定された弾の種類のレベルを1上げる
	switch (bulletType)
	{
	case BulletType::LineBullet:
		if (lineLevel_ < maxBulLevel_)
		{
			lineLevel_ += 1;
			UIManager::GetInstance()->PlusUIBul("lineUI");
		}
		break;
	case BulletType::SideBullet:
		if (sideLevel_ < maxBulLevel_)
		{
			sideLevel_ += 1;
			UIManager::GetInstance()->PlusUIBul("sideUI");
		}
		break;
	case BulletType::WideBullet:
		if (wideLevel_ < maxBulLevel_)
		{
			wideLevel_ += 1;
			UIManager::GetInstance()->PlusUIBul("wideUI");
		}
		break;
		/*case BulletType::Roket:
		if (wideLevel_ < maxBulLevel_)
		{
			roketLevel_ += 1;
		}
		break;*/
	default:
		break;
	}
}

void Player::SetIsAlive(bool isAlive)
{
	isAlive_ = isAlive;
	//死んだならコントローラー振動させる
	if (isAlive_ == false)
	{
		NInput::Vibration(0.8f, 0.8f, 0.3f);
	}
}
