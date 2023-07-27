#pragma once
#include "json.hpp"
#include "NObj3d.h"
#include "NCamera.h"
#include <unordered_map>

typedef std::string ModelName;

struct LevelData
{
	struct ObjectData
	{
		std::string filename;
		NVector3 trans;
		NVector3 rot;
		NVector3 scale;
	};

	NCamera camera;
	std::list<ObjectData> objects;
	std::unordered_map<ModelName, Model> models;
};

class NLevelDataLoader final
{
private:


public:
	//シングルトンインスタンス取得
	static NLevelDataLoader* GetInstance();
	std::unique_ptr<LevelData> Load(const std::string& filename);
	//走査
	void Traversal(nlohmann::json& object, LevelData* levelData);

	void TraversalCamera(nlohmann::json& camera, LevelData* levelData);

	//レベルエディタから出力された読み込み済オブジェクトを配置していく
	void SetObject(const LevelData* levelData, std::vector<std::unique_ptr<NObj3d>> &obj);

	//カメラを設定する
	NCamera SetCamera(const LevelData* levelData);

private:
	NLevelDataLoader() = default;
	NLevelDataLoader(const NLevelDataLoader&) = delete;
	~NLevelDataLoader() = default;
	NLevelDataLoader& operator=(const NLevelDataLoader&) = delete;
};

