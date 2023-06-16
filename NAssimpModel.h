#pragma once
#include "NAssimpLoader.h"
#include "NConstBuff.h"
#include "NMaterial.h"
#include <memory>

class NAssimpModel
{
private:
	std::vector<Mesh> meshes_; // メッシュの配列
	std::vector<NVertexBuff> vertexBuffers_; // メッシュの数分の頂点バッファ
	std::vector<NIndexBuff> indexBuffers_; // メッシュの数分のインデックスバッファ
	const wchar_t* kModelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";


	//いるかわからんけど仮で
	NMatrix4 matWorld_;		//3D変換行列
	NMaterial material_;	//マテリアル
	NColor color_;			//色

	ImportSettings importSetting_ = // これ自体は自作の読み込み設定構造体
	{
		kModelFile,
		meshes_,
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
	//ファイルからメッシュ情報を読み取り、それに応じたバッファを生成する
	void Load();
	void Init();
	void Update();
	void Draw();
};