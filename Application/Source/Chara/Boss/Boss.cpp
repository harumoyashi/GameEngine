#include "Boss.h"
#include "Player.h"
#include "Wave.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "ItemManager.h"
#include "NCameraManager.h"
#include "Score.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

Boss::Boss() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), isAlive_(true), homingPower_(0.2f),
	elapseSpeed_(0.0f), maxHP_(10), hp_(maxHP_), score_(300)
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
	obj_->scale_ = NVec3::zero;
	obj_->rotation_ = NVec3::zero;
	obj_->color_ = NColor::kItem;
	obj_->Update();

	//コライダー設定
	collider_.SetCenterPos({ obj_->position_.x,obj_->position_.z });
	collider_.SetRadius(oriScale_.x);
	collider_.SetColID("boss");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Boss::OnCollision, this));

	isAlive_ = true;
	hp_ = maxHP_;
	moveSpeed_ = Player::GetInstance()->GetMoveSpeed() * 1.2f;

	entryTimer_.Start();
	////演出用にカメラ遷移する
	NCameraManager::GetInstance()->ChangeCameara(CameraType::BossEntry);
}

void Boss::Init()
{
	isAlive_ = false;
	moveAngle_ = 0.f;

	entryTimer_.Reset();
	scalingTimer_.Reset();

	bossTex_ = std::make_unique<NSprite>();
	bossTex_->CreateSprite("boss");
	bossTex_->SetAncor({ 0.5f,0.5f });
	bossTex_->SetSize(700.f, 200.f);
	bossTex_->rotation_ = -15.0f;
	bossTex_->SetPos(
		-(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	bossTex_->color_.SetColor255(255, 255, 255, 100);

	bossTexStartPos = {
		-(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height };
	bossTexEndPos = {
		(float)NWindows::GetInstance()->kWin_width + (float)NWindows::GetInstance()->kWin_width * 0.5f,
		0.f };
	bossTexNowPos = bossTexStartPos;

	bossInTimer_.Reset();
	bossOutTimer_.Reset();
}

void Boss::Update()
{
	entryTimer_.Update();
	scalingTimer_.Update();

	if (entryTimer_.GetRun())
	{
		//演出中は波とプレイヤーの動き止める
		Wave::GetInstance()->SetIsMove(false);
		Player::GetInstance()->SetElapseSpeed(0.f);
		Player::GetInstance()->SetIsMove(false);

		//カメラの遷移も終わったタイミングで文字出し始める
		if (bossInTimer_.GetStarted() == false && NCameraManager::GetInstance()->GetIsEntryCameraChanged())
		{
			bossInTimer_.Start();
			NAudioManager::GetInstance()->Play("mutekiSE");	//音も鳴らす
		}

		//文字スライドさせて出現
		if (bossInTimer_.GetRun())
		{
			bossTexNowPos.x = NEasing::InOutBack(
				bossTexStartPos.x,
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				bossInTimer_.GetTimeRate());
			bossTexNowPos.y = NEasing::InOutBack(
				bossTexStartPos.y,
				(float)NWindows::GetInstance()->kWin_height * 0.5f,
				bossInTimer_.GetTimeRate());
		}

		//咆哮前のズームアウト終わったら咆哮音鳴らす
		if (NCameraManager::GetInstance()->GetIsEntryCameraZoomOutEnd() &&
			NAudioManager::GetInstance()->GetIsPlaying("shoutSE") == false)
		{
			NAudioManager::GetInstance()->Play("shoutSE");
			NInput::GetInstance()->Vibration(0.8f,0.8f,0.5f);
		}
	}
	//演出終わったら元のカメラに戻す
	else if (entryTimer_.GetEnd())
	{
		//演出終わったら波とプレイヤーの動き戻す
		Wave::GetInstance()->SetIsMove(true);
		Player::GetInstance()->SetIsMove(true);
		entryTimer_.Reset();
		NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
	}

	//演出が終わったら掃けさせる
	if (bossOutTimer_.GetStarted() == false && bossInTimer_.GetEnd() &&
		NCameraManager::GetInstance()->GetNowCameraType() == CameraType::Normal)
	{
		bossOutTimer_.Start();
	}

	//文字スライドさせて掃けさす
	if (bossOutTimer_.GetRun())
	{
		bossTexNowPos.x = NEasing::OutQuad(
			(float)NWindows::GetInstance()->kWin_width * 0.5f,
			bossTexEndPos.x,
			bossOutTimer_.GetTimeRate());
		bossTexNowPos.y = NEasing::OutQuad(
			(float)NWindows::GetInstance()->kWin_height * 0.5f,
			bossTexEndPos.y,
			bossOutTimer_.GetTimeRate());
	}

	//全部終わったらリセット
	if (bossOutTimer_.GetEnd())
	{
		bossInTimer_.Reset();
		bossOutTimer_.Reset();
	}

	bossTex_->SetPos(bossTexNowPos.x, bossTexNowPos.y);
	bossInTimer_.Update();
	bossOutTimer_.Update();

	//だんだん大きく
	if (NCameraManager::GetInstance()->GetIsEntryCameraChanged() && scalingTimer_.GetStarted() == false)
	{
		scalingTimer_.Start();
	}

	if (scalingTimer_.GetRun())
	{
		obj_->scale_ = MathUtil::OutBack(NVec3::zero, oriScale_, scalingTimer_.GetTimeRate());
	}
	else if (scalingTimer_.GetEnd())
	{
		obj_->scale_ = oriScale_;	//一応タイマー終わったら元の大きさにする
	}

	if (isAlive_)
	{
		//経過時間を適用
		SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

		if (entryTimer_.GetRun() == false)
		{
			//移動
			Move();
		}

		obj_->Update();
		collider_.Update(obj_.get());

		if (hp_ <= 0)
		{
			DeadParticle();
			Score::AddScore(score_);
			isAlive_ = false;
			//アイテム落とす
			uint32_t bulType = MathUtil::Random(0, (uint32_t)BulletType::MaxType);
			ItemManager::GetInstance()->Generate(obj_->position_, (BulletType)bulType);

			NAudioManager::GetInstance()->Play("vanishSE");
		}
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

void Boss::DrawSprite()
{
	bossTex_->Draw();
}

void Boss::OnCollision()
{
	//当たった相手が弾だった時の処理
	if (collider_.GetColInfo()->GetColID() == "bullet" && hp_ > 0)
	{
		hp_--;

		//弾当たった時の音鳴らす
		NAudioManager::GetInstance()->Play("hitSE", false, 0.4f);
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
	//プレイヤーからボスのベクトル
	NVec3 p2bVec = obj_->position_ - Player::GetInstance()->GetPos();
	//外積を取ってプレイヤーに対して右か左か判定
	float isLR = moveVelo_.Cross({ p2bVec.x,p2bVec.z });

	//動く角度をプレイヤー側に傾ける
	moveAngle_ += isLR * homingPower_;

	//行きすぎたら0に戻す
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
