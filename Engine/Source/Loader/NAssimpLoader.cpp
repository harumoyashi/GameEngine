#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>
#include "NAssimpLoader.h"
#include "NUtil.h"

////ディレクトリパスのみにする
//std::wstring GetDirectoryPath(const std::wstring& origin)
//{
//	std::filesystem::path p = origin.c_str();
//	return p.remove_filename().c_str();
//}
//
//// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
//std::wstring ToWideString(const std::string& str)
//{
//	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
//
//	std::wstring wstr;
//	wstr.resize(num1);
//
//	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);
//
//	assert(num1 == num2);
//	return wstr;
//}
//
//bool NAssimpLoader::Load(const ImportSettings& settings)
//{
//	if (settings.filename == nullptr)
//	{
//		return false;
//	}
//
//	auto& meshes = settings.meshes;
//	auto inverseU = settings.inverseU;
//	auto inverseV = settings.inverseV;
//
//	auto path = NUtil::ToUTF8(settings.filename);
//
//	Assimp::Importer importer;
//	uint32_t flag = 0;
//	flag |= aiProcess_Triangulate;
//	flag |= aiProcess_PreTransformVertices;
//	flag |= aiProcess_CalcTangentSpace;
//	flag |= aiProcess_GenSmoothNormals;
//	flag |= aiProcess_GenUVCoords;
//	flag |= aiProcess_RemoveRedundantMaterials;
//	flag |= aiProcess_OptimizeMeshes;
//
//	auto scene = importer.ReadFile(path, flag);
//
//	if (scene == nullptr)
//	{
//		// もし読み込みエラーがでたら表示する
//		printf(importer.GetErrorString());
//		printf("\n");
//		return false;
//	}
//
//	// 読み込んだデータを自分で定義したMesh構造体に変換する
//	meshes.clear();
//	meshes.resize(scene->mNumMeshes);
//	for (size_t i = 0; i < meshes.size(); ++i)
//	{
//		const auto pMesh = scene->mMeshes[i];
//		LoadMesh(meshes[i], pMesh, inverseU, inverseV);
//		const auto pMaterial = scene->mMaterials[i];
//		LoadTexture(settings.filename, meshes[i], pMaterial);
//	}
//
//	scene = nullptr;
//
//	return true;
//}
//
//void NAssimpLoader::LoadMesh(NMesh& dst, const aiMesh* src, bool inverseU, bool inverseV)
//{
//	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
//	aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//	dst.vertices.resize(src->mNumVertices);
//
//	for (auto i = 0u; i < src->mNumVertices; ++i)
//	{
//		auto position = &(src->mVertices[i]);
//		auto normal = &(src->mNormals[i]);
//		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
//		auto boneIndex = (src->mBones[i]->mNumWeights);
//		auto boneWeight = (src->mBones[i]->mWeights);
//
//		// 反転オプションがあったらUVを反転させる
//		if (inverseU)
//		{
//			uv->x = 1 - uv->x;
//		}
//		if (inverseV)
//		{
//			uv->y = 1 - uv->y;
//		}
//
//		NVertexFbx vertex = {};
//		vertex.pos = NVec3(position->x, position->y, position->z);
//		vertex.normal = NVec3(normal->x, normal->y, normal->z);
//		vertex.uv = NVec2(uv->x, uv->y);
//		vertex.boneIndex[0] = 0;
//		vertex.boneWeight = {1.f,0.f,0.f};
//
//		dst.vertices[i] = vertex;
//	}
//
//	dst.indices.resize(src->mNumFaces * 3);
//
//	for (auto i = 0u; i < src->mNumFaces; ++i)
//	{
//		const auto& face = src->mFaces[i];
//
//		dst.indices[i * 3 + 0] = face.mIndices[0];
//		dst.indices[i * 3 + 1] = face.mIndices[1];
//		dst.indices[i * 3 + 2] = face.mIndices[2];
//	}
//
//	indexBuff_.Init(dst.indices);
//	vertexBuff_.Init(dst.vertices);
//}
//
//void NAssimpLoader::LoadBone(Bone& dst, const aiMesh* src)
//{
//	for (auto i = 0u; i < src->mNumVertices; ++i)
//	{
//		aiBone* bone = src->mBones[i];
//
//		dst.offsetMatrix.m[0][0] = (float)bone->mOffsetMatrix.a1;
//		dst.offsetMatrix.m[0][1] = (float)bone->mOffsetMatrix.a2;
//		dst.offsetMatrix.m[0][2] = (float)bone->mOffsetMatrix.a3;
//		dst.offsetMatrix.m[0][3] = (float)bone->mOffsetMatrix.a4;
//		dst.offsetMatrix.m[1][0] = (float)bone->mOffsetMatrix.b1;
//		dst.offsetMatrix.m[1][1] = (float)bone->mOffsetMatrix.b2;
//		dst.offsetMatrix.m[1][2] = (float)bone->mOffsetMatrix.b3;
//		dst.offsetMatrix.m[1][3] = (float)bone->mOffsetMatrix.b4;
//		dst.offsetMatrix.m[2][0] = (float)bone->mOffsetMatrix.c1;
//		dst.offsetMatrix.m[2][1] = (float)bone->mOffsetMatrix.c2;
//		dst.offsetMatrix.m[2][2] = (float)bone->mOffsetMatrix.c3;
//		dst.offsetMatrix.m[2][3] = (float)bone->mOffsetMatrix.c4;
//		dst.offsetMatrix.m[3][0] = (float)bone->mOffsetMatrix.d1;
//		dst.offsetMatrix.m[3][1] = (float)bone->mOffsetMatrix.d2;
//		dst.offsetMatrix.m[3][2] = (float)bone->mOffsetMatrix.d3;
//		dst.offsetMatrix.m[3][3] = (float)bone->mOffsetMatrix.d4;
//	}
//	dst.indices.resize(src->mNumFaces * 3);
//	for (auto i = 0u; i < src->mNumFaces; ++i)
//	{
//		const auto& face = src->mFaces[i];
//		dst.indices[i * 3 + 0] = face.mIndices[0];
//		dst.indices[i * 3 + 1] = face.mIndices[1];
//		dst.indices[i * 3 + 2] = face.mIndices[2];
//	}
//}
//
//void NAssimpLoader::LoadTexture(const wchar_t* filename, NMesh& dst, const aiMaterial* src)
//{
//	aiString path;
//	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
//	{
//		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
//		auto dir_ = GetDirectoryPath(filename);
//		auto file = std::string(path.C_Str());
//		dst.textureName = dir_ + ToWideString(file);
//	}
//	else
//	{
//		dst.textureName.clear();
//	}
//}

void NAssimpLoader::ParseMesh(FbxModel* model, aiMesh* mesh)
{
	ParseVertex(model, mesh);
	ParseFace(model, mesh);
	ParseSkin(model, mesh);
}

void NAssimpLoader::ParseVertex(FbxModel* model, aiMesh* mesh)
{
	std::vector<NVertexFbx>& vertex = model->mesh.vertices;
	vertex.resize(mesh->mNumVertices);	//メッシュの頂点数に合わせて要素確保する

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		// 頂点座標
		vertex[i].pos.x = mesh->mVertices[i].x;
		vertex[i].pos.y = mesh->mVertices[i].y;
		vertex[i].pos.z = mesh->mVertices[i].z;

		// 法線ベクトル
		vertex[i].normal.x = mesh->mNormals[i].x;
		vertex[i].normal.y = mesh->mNormals[i].y;
		vertex[i].normal.z = mesh->mNormals[i].z;

		// uv座標
		vertex[i].uv.x = mesh->mTextureCoords[0][i].x;
		vertex[i].uv.y = -mesh->mTextureCoords[0][i].y;	//fbxはvが反転してるっぽいから直してあげる
	}
}

void NAssimpLoader::ParseFace(FbxModel* model, aiMesh* mesh)
{
	std::vector<uint32_t>& indices = model->mesh.indices;
	indices.resize(mesh->mNumFaces * 3);

	// フェイス
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices[i * 3 + j] = face.mIndices[j];
		}
	}
}

void NAssimpLoader::ParseSkin(FbxModel* model, aiMesh* mesh)
{
	// スキニング情報を持つメッシュかどうか確認
	if (mesh->HasBones() == true)
	{
		// ボーン番号とスキンウェイトのペア
		struct WeightSet
		{
			uint32_t index;
			float weight;
		};

		// 二次元配列（ジャグ配列） list:頂点が影響を受けるボーンの全リスト vector:それを全頂点分
		std::vector<std::list<WeightSet>> weightLists(model->mesh.vertices.size());

		// ボーンの最大数設定
		model->bones.resize(mesh->mNumBones);

		// スキニング情報の処理
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			//ボーン情報格納
			aiBone* bone = mesh->mBones[i];

			// ボーンの名前登録
			model->bones[i].name = bone->mName.C_Str();

			// ボーンの初期姿勢行列(バインドポーズ行列)
			NMatrix4 initalMat = AssimpMatToMat4(bone->mOffsetMatrix);	//自作の型に変換
			model->bones[i].offsetMat = initalMat.Transpose();	//バインドポーズ行列の転置行列(逆行列かも)

			// ウェイトの読み取り
			for (uint32_t j = 0; j < bone->mNumWeights; j++)
			{
				// 頂点番号
				uint32_t vertexIndex = bone->mWeights[j].mVertexId;
				// スキンウェイト
				float weight = bone->mWeights[j].mWeight;
				// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
				weightLists[vertexIndex].emplace_back(WeightSet{ i,weight });
			}
		}

		// ウェイトの整理 //
		// メッシュの頂点情報格納
		auto& vertices = model->mesh.vertices;
		// 各頂点について処理
		for (uint32_t i = 0; i < vertices.size(); i++)
		{
			// 頂点のウェイトから最も大きい4つを選択
			auto& weightList = weightLists[i];
			// 大小比較用のラムダ式を指定して降順にソート
			weightList.sort(
				[](auto const& lhs, auto const& rhs)
				{
					return lhs.weight > rhs.weight;
				});

			uint32_t weightArrayIndex = 0;
			// 降順ソート済みのウェイトリストから
			for (auto& weightSet : weightList)
			{
				// 頂点データに書き込み
				vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
				vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
				// 4つに達したら終了
				if (++weightArrayIndex >= kMaxBoneIndices)
				{
					float weight = 0.f;
					// 2番目以降のウェイトを合計
					for (size_t j = 1; j < kMaxBoneIndices; j++)
					{
						weight += vertices[i].boneWeight[j];
					}
					// 合計で1.0f(100%)になるように調整
					vertices[i].boneWeight[0] = 1.f - weight;
					break;
				}
			}
		}
	}
}

void NAssimpLoader::ParseMaterial(FbxModel* model, const aiScene* scene)
{
	for (uint32_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		// マテリアル名
		aiString materialName;
		if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS)
		{
			model->material.name = materialName.C_Str();
		}

		// アンビエントカラー
		aiColor3D ambientColor;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
		{
			model->material.ambient.x = ambientColor.r;
			model->material.ambient.y = ambientColor.g;
			model->material.ambient.z = ambientColor.b;
		}

		// ディフューズカラー
		aiColor3D diffuseColor;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
		{
			model->material.diffuse.x = diffuseColor.r;
			model->material.diffuse.y = diffuseColor.g;
			model->material.diffuse.z = diffuseColor.b;
		}

		// テクスチャー
		uint32_t textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
		for (uint32_t j = 0; j < textureCount; j++)
		{
			aiString texturePath;

			if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
			{
				const std::string filePath = texturePath.C_Str();

				std::string pathStr(filePath);
				std::string name = ExractFileName(pathStr);
				std::string baseDirectory = "Resources/Model/";
				std::string fullPath = baseDirectory + model->name + "/" + name;
				std::string textureTag = model->name + "Tex";

				// テクスチャ読み込み
				model->material.texture = NTextureManager::GetInstance()->LoadTexture(fullPath, textureTag);
			}
		}
	}
}

void NAssimpLoader::ParseNodeRecursive(FbxModel* model, Node* parent, const aiNode* node)
{
	aiString nodeName = node->mName;

	// モデルにノードを追加
	model->nodes.emplace_back();
	Node& modelNode = model->nodes.back();

	// ノード名を取得
	modelNode.name = node->mName.C_Str();

	// ローカル行列
	modelNode.localTransformMat = AssimpMatToMat4(node->mTransformation);

	// グローバル行列
	modelNode.globalTransformMat = modelNode.localTransformMat;
	if (parent)
	{
		modelNode.parent = parent;
		// 親の行列を掛けてあげる
		modelNode.globalTransformMat *= parent->globalTransformMat;
	}

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = model->scene->mMeshes[node->mMeshes[i]];
		if (aimesh)
		{
			ParseMesh(model, aimesh);
		}
	}

	// 再帰
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ParseNodeRecursive(model, &modelNode, node->mChildren[i]);
	}
}

NAssimpLoader* NAssimpLoader::GetInstance()
{
	static NAssimpLoader instance;
	return &instance;
}

bool NAssimpLoader::Load(const std::string& filePath, FbxModel* model)
{
	// フラグ
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// シーンの読み込み
	model->scene = model->importer.ReadFile(filePath, flags);

	if (model->scene == nullptr)
	{
		assert(0 && "モデルの読み込みに失敗しました");
		return false;
	}

	// マテリアルの解析
	ParseMaterial(model, model->scene);

	ParseNodeRecursive(model, nullptr, model->scene->mRootNode);

	return true;
}

NMatrix4 NAssimpLoader::AssimpMatToMat4(const aiMatrix4x4& mat)
{
	NMatrix4 result;

	for (uint32_t i = 0; i < 4; i++)
	{
		for (uint32_t j = 0; j < 4; j++)
		{
			result.m[i][j] = (float)mat[i][j];
		}
	}

	return result;
}

std::string NAssimpLoader::ExractFileName(const std::string& path)
{
	size_t pos1;
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
