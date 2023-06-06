#include "LevelDataLoader.h"
#include <string>
#include <fstream>
#include <assert.h>

LevelDataLoader* LevelDataLoader::GetInstance()
{
	static LevelDataLoader instance;
	return &instance;
}

std::unique_ptr<LevelData> LevelDataLoader::Load(const std::string& directoryPath, const std::string& filename)
{
	//連結してフルパスを見る
	const std::string fullpath = directoryPath + filename;

	//ファイルストリーム
	std::ifstream file;

	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();

	//"objects"のオブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		Traversal(object, levelData.get());

		//再起呼び出しで枝を走査する
		for (nlohmann::json& c : object["children"])
		{
			Traversal(c, levelData.get());
		}
	}
	return levelData;
}

void LevelDataLoader::Traversal(nlohmann::json& object, LevelData* levelData)
{
	assert(object.contains("type"));

	//種別を取得
	std::string type = object["type"].get<std::string>();

	//種類ごとの処理
	//MESH
	if (type.compare("MESH") == 0)
	{
		//要素追加
		levelData->objects.emplace_back(LevelData::ObjectData{});
		//今追加した要素の参照を得る
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name"))
		{
			//ファイル名
			objectData.filename = object["file_name"];
		}

		//トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];

		//一旦Vec3に置き換える
		objectData.trans = { (float)transform["translation"][1],(float)transform["translation"][2],-(float)transform["translation"][0] };
		objectData.rot = { -(float)transform["rotation"][1],-(float)transform["rotation"][2],(float)transform["rotation"][0] };
		objectData.scale = { (float)transform["scaling"][1],(float)transform["scaling"][2],(float)transform["scaling"][0] };
	}
}
