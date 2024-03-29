#pragma once
#include "json.hpp"
#include "NObj3d.h"
#include "NCamera.h"
#include <unordered_map>

// レベルエディタ読み込み //

typedef std::string ModelName;

struct LevelData
{
	struct ObjectData
	{
		std::string filename;
		NVec3 trans;
		NVec3 rot;
		NVec3 scale;
	};

	NCamera camera;
	std::list<ObjectData> objects;
	std::unordered_map<ModelName, IModel> models;
};

class NLevelDataLoader final
{
private:


public:
	//シングルトンインスタンス取得
	static NLevelDataLoader* GetInstance();
	//レベルエディタの読み込み
	std::unique_ptr<LevelData> Load(const std::string& filename);
	//走査
	void Traversal(nlohmann::json& object, LevelData* levelData);

	//レベルエディタから出力された読み込み済オブジェクトを配置していく
	void SetObject(const LevelData* levelData, std::vector<std::unique_ptr<NObj3d>>& obj);

	//カメラを設定する
	NCamera SetCamera(const LevelData* levelData);

private:
	//コンストラクタ
	NLevelDataLoader() = default;
	//コピー禁止
	NLevelDataLoader(const NLevelDataLoader&) = delete;
	//デストラクタ
	~NLevelDataLoader() = default;
	//コピー禁止
	NLevelDataLoader& operator=(const NLevelDataLoader&) = delete;
};

