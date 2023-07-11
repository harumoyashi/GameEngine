#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3dx12.h>
#include <filesystem>
#include "NAssimpLoader.h"
#include "NUtil.h"

//ディレクトリパスのみにする
std::wstring GetDirectoryPath(const std::wstring& origin)
{
	std::filesystem::path p = origin.c_str();
	return p.remove_filename().c_str();
}

// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

bool NAssimpLoader::Load(const ImportSettings& settings)
{
	if (settings.filename == nullptr)
	{
		return false;
	}

	auto& meshes = settings.meshes;
	auto inverseU = settings.inverseU;
	auto inverseV = settings.inverseV;

	auto path = NUtil::ToUTF8(settings.filename);

	Assimp::Importer importer;
	uint32_t flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	/*flag |= aiProcess_Triangulate;
    flag |= aiProcess_JoinIdenticalVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_TransformUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;
    flag |= aiProcess_LimitBoneWeights;*/

	auto scene = importer.ReadFile(path, flag);

	if (scene == nullptr)
	{
		// もし読み込みエラーがでたら表示する
		printf(importer.GetErrorString());
		printf("\n");
		return false;
	}

	// 読み込んだデータを自分で定義したMesh構造体に変換する
	meshes.clear();
	meshes.resize(scene->mNumMeshes);
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshes[i], pMesh, inverseU, inverseV);
		const auto pMaterial = scene->mMaterials[i];
		LoadTexture(settings.filename, meshes[i], pMaterial);
	}

	scene = nullptr;

	return true;
}

void NAssimpLoader::LoadMesh(Mesh& dst, const aiMesh* src, const bool inverseU, const bool inverseV)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

	dst.vertices.resize(src->mNumVertices);

	for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		/*auto boneIndex = (src->mBones[i]->mNumWeights);
		auto boneWeight = (src->mBones[i]->mWeights);*/

		// 反転オプションがあったらUVを反転させる
		if (inverseU)
		{
			uv->x = 1 - uv->x;
		}
		if (inverseV)
		{
			uv->y = 1 - uv->y;
		}

		NVertexPNU vertex = {};
		vertex.pos = NVector3(position->x, position->y, position->z);
		vertex.normal = NVector3(normal->x, normal->y, normal->z);
		vertex.uv = NVector2(uv->x, uv->y);
		/*vertex.boneIndex = boneIndex;
		vertex.boneWeight = boneWeight;*/

		dst.vertices[i] = vertex;
	}

	dst.indices.resize(src->mNumFaces * 3);

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.indices[i * 3 + 0] = face.mIndices[0];
		dst.indices[i * 3 + 1] = face.mIndices[1];
		dst.indices[i * 3 + 2] = face.mIndices[2];
	}

	indexBuff_.Init(dst.indices);
	vertexBuff_.Init(dst.vertices);
}

void NAssimpLoader::LoadBone(Bone& dst, const aiMesh* src)
{
	/*for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		aiBone* bone = src->mBones[i];
		

		dst.ofstd::filesystemetMatrix.m[0][0] = (float)bone->mOfstd::filesystemetMatrix.a1;
		dst.ofstd::filesystemetMatrix.m[0][1] = (float)bone->mOfstd::filesystemetMatrix.a2;
		dst.ofstd::filesystemetMatrix.m[0][2] = (float)bone->mOfstd::filesystemetMatrix.a3;
		dst.ofstd::filesystemetMatrix.m[0][3] = (float)bone->mOfstd::filesystemetMatrix.a4;
		dst.ofstd::filesystemetMatrix.m[1][0] = (float)bone->mOfstd::filesystemetMatrix.b1;
		dst.ofstd::filesystemetMatrix.m[1][1] = (float)bone->mOfstd::filesystemetMatrix.b2;
		dst.ofstd::filesystemetMatrix.m[1][2] = (float)bone->mOfstd::filesystemetMatrix.b3;
		dst.ofstd::filesystemetMatrix.m[1][3] = (float)bone->mOfstd::filesystemetMatrix.b4;
		dst.ofstd::filesystemetMatrix.m[2][0] = (float)bone->mOfstd::filesystemetMatrix.c1;
		dst.ofstd::filesystemetMatrix.m[2][1] = (float)bone->mOfstd::filesystemetMatrix.c2;
		dst.ofstd::filesystemetMatrix.m[2][2] = (float)bone->mOfstd::filesystemetMatrix.c3;
		dst.ofstd::filesystemetMatrix.m[2][3] = (float)bone->mOfstd::filesystemetMatrix.c4;
		dst.ofstd::filesystemetMatrix.m[3][0] = (float)bone->mOfstd::filesystemetMatrix.d1;
		dst.ofstd::filesystemetMatrix.m[3][1] = (float)bone->mOfstd::filesystemetMatrix.d2;
		dst.ofstd::filesystemetMatrix.m[3][2] = (float)bone->mOfstd::filesystemetMatrix.d3;
		dst.ofstd::filesystemetMatrix.m[3][3] = (float)bone->mOfstd::filesystemetMatrix.d4;
	}

	dst.indices.resize(src->mNumFaces * 3);

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.indices[i * 3 + 0] = face.mIndices[0];
		dst.indices[i * 3 + 1] = face.mIndices[1];
		dst.indices[i * 3 + 2] = face.mIndices[2];
	}*/
}

void NAssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
		auto dir_ = GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		dst.textureName = dir_ + ToWideString(file);
	}
	else
	{
		dst.textureName.clear();
	}
}
