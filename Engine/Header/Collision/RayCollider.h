#pragma once
#include "NBaseCollider.h"

// レイ形のコライダー //

class RayCollider final :
	public NBaseCollider
{
private:
	NVec3 startPos_;
	NVec3 dirVec_;

public:
	//コンストラクタ
	RayCollider(NVec3 startPos = NVec3::zero, NVec3 dirVec = NVec3::zero) :
		startPos_(startPos), dirVec_(dirVec)
	{
		shapeType_ = COL_RAY;
	}

	//更新
	void Update(NObj3d* obj)override;

	// ゲッター //
	//原点からの距離取得
	const NVec3& GetStartPos()const { return startPos_; }
	//法線取得
	const NVec3& GetDirVec()const { return dirVec_; }

	// セッター //
	//原点からの距離設定
	void SetStartPos(const NVec3& startPos) { startPos_ = startPos; }
	//法線設定
	void SetDirVec(const NVec3& dirVec) { dirVec_ = dirVec; }
};

