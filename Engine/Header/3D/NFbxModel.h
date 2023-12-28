#pragma once
#include "IModel.h"
#include "NTimer.h"

#include <assimp/Importer.hpp>

// FBXモデルのひな形 //

struct Bone
{
	std::string name;

	NMatrix4 offsetMat;
	NMatrix4 currentMat;
};

typedef struct Animation final
{
	uint32_t index = 0;
	NTimer timer;
	bool isPlay = false;
}Animation;

typedef struct Node final
{
	std::string name;
	NVec3 pos = NVec3::zero;
	NVec3 scale = NVec3::one;
	NVec3 rot = NVec3::zero;
	NMatrix4 localTransformMat;
	NMatrix4 globalTransformMat;
	Node* parent = nullptr;
}Node;

//前方宣言
struct aiNodeAnim;
struct aiAnimation;
struct aiNode;
struct aiScene;

struct FbxModel final
	: public IModel
{
	std::vector<Bone> bones;
	std::vector<Node> nodes;
	Animation animation;

	Assimp::Importer importer;
	const aiScene* scene;

	//コンストラクタ
	FbxModel();

	//アニメーションの再生
	void PlayAnimation(bool isElapse);
	//指定された名前のアニメーションノードを探す
	aiNodeAnim* FindNodeAnimation(const std::string& nodeName, aiAnimation* anime);

	//スケールインデックスを取得
	uint32_t FindScaleIndex(const aiNodeAnim* nodeAnim, const float nowTime);
	//回転インデックスを取得
	uint32_t FindRotIndex(const aiNodeAnim* nodeAnim, const float nowTime);
	//座標インデックスを取得
	uint32_t FindPosIndex(const aiNodeAnim* nodeAnim, const float nowTime);

	//スケーリングを補完
	NVec3 CalcCurrentScale(const aiNodeAnim* nodeAnim, const float nowTime);
	//回転を補完
	NQuaternion CalcCurrentRot(const aiNodeAnim* nodeAnim, const float nowTime);
	//座標を補完
	NVec3 CalcCurrentPos(const aiNodeAnim* nodeAnim, const float nowTime);
	//アニメーションのキーに応じて変形
	void ParseNodeHeirarchy(const float nowTime, const uint32_t index, const NMatrix4& parentMat, const aiNode* rootNode);

	// セッター //
	//アニメーション再生フラグ設定
	void SetIsPlayAnime(bool isPlay) { animation.isPlay = isPlay; }
};