#pragma once
#include "NAssimpLoader.h"
#include "NConstBuff.h"
#include "NMaterial.h"
#include "NLightGroup.h"
#include <memory>

class NAssimpModel final
{
private:
	std::vector<Mesh> meshes_;	// メッシュの配列
	//std::vector<Bone> bones_;	// ボーンの配列
	std::vector<NVertexBuff<NVertexPNU>> vertexBuffers_; // メッシュの数分の頂点バッファ
	std::vector<NIndexBuff> indexBuffers_; // メッシュの数分のインデックスバッファ
	const wchar_t* kModelFile;

	//いるかわからんけど仮で
	NMatrix4 matWorld_;		//3D変換行列
	NMaterial material_;	//マテリアル
	NColor color_;			//色

	//ライトたち
	static NLightGroup* sLightGroup;

	ImportSettings importSetting_ = // これ自体は自作の読み込み設定構造体
	{
		kModelFile,
		meshes_,
		//bones_,
		false,
		true // アリシアのモデルは、テクスチャのUVのVだけ反転してるっぽい？ので読み込み時にUV座標を逆転させる
	};

	//assimpのローダー
	NAssimpLoader loader_;

	//定数バッファども
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;
	std::unique_ptr<NConstBuff<ConstBuffDataMaterial>> cbMaterial_;
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;

public:
	NVector3 scale_ = { 1.0f,1.0f,1.0f };		//スケーリング倍率
	NVector3 rotation_ = { 0.0f,0.0f,0.0f };	//回転角
	NVector3 position_ = { 0.0f,0.0f,0.0f };	//座標

	//ボーンインデックスの最大数
	static const uint32_t MAX_BONE_INDICES = 4;

public:
	//ファイルからメッシュ情報を読み取り、それに応じたバッファを生成する
	void Load(const wchar_t* filename);
	void Init();
	void Update();
	void Draw();

	inline static void SetLightGroup(NLightGroup* lightGroup) { sLightGroup = lightGroup; }
};