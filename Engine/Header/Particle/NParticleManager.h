#pragma once
#include "SimpleParticle.h"
#include <unordered_map>

// パーティクルの管理 //
// エミッターを登録して更新処理をまとめてかけてる //

class IEmitter3D;

class NParticleManager final
{
public:
	std::unordered_map<std::string, IEmitter3D*> emitters_;			//パーティクルエミッター群
	std::unordered_map<std::string, IEmitter3D*> polygonEmitters_;	//ポリゴンパーティクルエミッター群
	std::vector<IEmitter3D*> enemyEmitters_;						//敵用パーティクルエミッター群
	std::vector<IEmitter3D*> bulletEmitters_;						//弾用パーティクルエミッター群

public:
	static NParticleManager* GetInstance();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//エミッターをunordered_mapに追加
	void AddEmitter(IEmitter3D* emitter, const std::string& key)
	{
		//形状によって登録するエミッター群の種類わける
		if (emitter->GetShapeType() == (uint32_t)ShapeType::Cube)
		{
			//同じ名前のやつが登録されてたら追加せずに処理終了
			for (auto& emit : emitters_)
			{
				if (emit.first == key)
				{
					return;
				}
			}
			emitter->Init();					//初期化してから登録
			emitters_.emplace(std::make_pair(key, emitter));
		}
		else if(emitter->GetShapeType() == (uint32_t)ShapeType::Polygon)
		{
			//同じ名前のやつが登録されてたら追加せずに処理終了
			for (auto& emit : polygonEmitters_)
			{
				if (emit.first == key)
				{
					return;
				}
			}
			emitter->Init();					//初期化してから登録
			polygonEmitters_.emplace(std::make_pair(key, emitter));
		}
	}
	//エミッターをunordered_mapから削除
	void EraseEmitter(const std::string& key)
	{
		emitters_.erase(key);
	}
	//有効フラグを設定
	void SetIsActive(const std::string& key, bool isActive)
	{
		emitters_[key]->SetIsActive(isActive);
	}

private:
	NParticleManager() = default;
	NParticleManager(const NParticleManager&) = delete;
	~NParticleManager() = default;
	NParticleManager& operator=(const NParticleManager&) = delete;
};