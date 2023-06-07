#pragma once
#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NModel.h"
#include "NAudio.h"
#include "NCamera.h"
#include "NCollision.h"
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NTimer.h"
#include "AssimpLoader.h"
#include "NConstBuff.h"
#include "LevelDataLoader.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NTitleScene
{
private:
	//オブジェクト
	NMaterial material;				//マテリアル
	static const uint32_t maxObj = 3;	//オブジェクト数
	std::vector<std::unique_ptr<NObj3d>> obj;	//オブジェクト
	std::vector<std::unique_ptr<NObj3d>> levelDataobj;	//レベルデータから読み込んだオブジェクト

	static const uint32_t maxModel = 2;	//モデル数
	std::vector<NModel> model;	//モデル情報

	NCollision::Sphere sphere;
	NCollision::Plane plane;
	bool isCol = false;

	//背景スプライト
	static const uint32_t maxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite[maxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t maxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite[maxForeSprite];	//前景スプライト

	NCamera camera;	//カメラ

	//オーディオ
	NAudio* audio = nullptr;
	static const uint32_t maxSoundData = 3;		//サウンドデータの最大数
	uint32_t soundData[maxSoundData] = {};	//サウンドデータ格納用

	//ライトたち
	std::unique_ptr<NDirectionalLight> directionalLight;
	std::unique_ptr<NPointLight > pointLight;
	std::unique_ptr<NSpotLight> spotLight;
	std::unique_ptr<NCircleShadow> circleShadow;

	NTimer timer;

	bool flag = true;
	float color = 0.0f;

	std::vector<Mesh> meshes; // メッシュの配列
	std::vector<NVertexBuff*> vertexBuffers; // メッシュの数分の頂点バッファ
	std::vector<NIndexBuff*> indexBuffers; // メッシュの数分のインデックスバッファ
	const wchar_t* modelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";

	ImportSettings importSetting = // これ自体は自作の読み込み設定構造体
	{
		modelFile,
		meshes,
		false,
		true // アリシアのモデルは、テクスチャのUVのVだけ反転してるっぽい？ので読み込み時にUV座標を逆転させる
	};
	
	AssimpLoader loader;
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans;

	std::unique_ptr<LevelData> levelData;

public:
	//インスタンス取得
	static NTitleScene* GetInstance();

	NTitleScene();
	~NTitleScene();

	void Init();
	void Update();
	void Draw();
	void Reset();
	void Finalize();

	//レベルエディタから出力された読み込み済オブジェクトを配置していく
	void SetObject(LevelData* levelData);
};