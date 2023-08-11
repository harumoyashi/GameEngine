#include <assimp/scene.h>
#include "NFbxModel.h"
#include "NAssimpLoader.h"


FbxModel::FbxModel()
{
	format = ModelFormat::Fbx;
}

void FbxModel::PlayAnimation()
{
	// アニメーション再生しない
	if (animation.isPlay == false)
	{
		//アニメーションタイマーを強制的に0にして再生されないように
		ParseNodeHeirarchy(0.f, 0, NMatrix4::Identity(), scene->mRootNode);
		return;
	}

	// 最大時間と現在の時間取得
	float maxTime = (float)scene->mAnimations[animation.index]->mDuration;
	float nowTime = animation.timer.GetTimeRate() * maxTime;

	//取得した現在のアニメーションタイマーを使ってアニメーションさせる
	ParseNodeHeirarchy(nowTime, animation.index, NMatrix4::Identity(), scene->mRootNode);

	//アニメーションタイマー回し続けとく
	animation.timer.Update(true);
}

aiNodeAnim* FbxModel::FindNodeAnimation(const std::string& nodeName, aiAnimation* animation)
{
	return nullptr;
}

uint32_t FbxModel::FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return 0;
}

uint32_t FbxModel::FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return 0;
}

uint32_t FbxModel::FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return 0;
}

NVec3 FbxModel::CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return NVec3();
}

NVec3 FbxModel::CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return NVec3();
}

NVec3 FbxModel::CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return NVec3();
}

void FbxModel::ParseNodeHeirarchy(const float currentTime, const uint32_t index, const NMatrix4& parentMat, const aiNode* rootNode)
{
	//aiMatrix4x4 aiMat = rootNode->mTransformation;

	//NMatrix4 currentPoseMat = NAssimpLoader::ConvertNMatrix4FromAssimpMat(aiMat).Transpose();

	//// ノードアニメーションを取得する
	//std::string nodeName = rootNode->mName.C_Str();
	//aiAnimation* animation = scene->mAnimations[index];  // 適切なアニメーションを選択する必要があります

	//const aiNodeAnim* nodeAnim = FindNodeAnim(nodeName, animation);

	//// ノードアニメーションがある場合、ノードの変換行列を補完する
	//if (nodeAnim)
	//{
	//	// スケーリングを補完
	//	Vec3 scale = CalcCurrentScale(nodeAnim, currentTime);

	//	// 回転を補完
	//	Quaternion rot = CalcCurrentRot(nodeAnim, currentTime);

	//	// 座標を補完
	//	Vec3 pos = CalcCurrentPos(nodeAnim, currentTime);

	//	// 行列の合成
	//	currentPoseMat = CalculateWorldMat(pos, scale, rot);
	//}

	//NMatrix4 globalTransformMat = currentPoseMat * parentMat;

	//for (uint32_t i = 0; i < bones.size(); i++)
	//{
	//	if (bones[i].name == nodeName)
	//	{
	//		NMatrix4 initalMat = bones[i].offsetMat;
	//		NMatrix4 invWorldMat = AssimpLoader::ConvertNMatrix4FromAssimpMat(scene->mRootNode->mTransformation);
	//		bones[i].currentMat = initalMat * globalTransformMat * invWorldMat;
	//	}
	//}

	//for (uint32_t i = 0; i < rootNode->mNumChildren; i++)
	//{
	//	ParseNodeHeirarchy(currentTime, index, globalTransformMat, rootNode->mChildren[i]);
	//}
}
