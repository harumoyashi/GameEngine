#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3dx12.h>
#include <filesystem>
#include "AssimpLoader.h"

namespace fs = std::filesystem;

//�f�B���N�g���p�X�݂̂ɂ���
std::wstring GetDirectoryPath(const std::wstring& origin)
{
    fs::path p = origin.c_str();
    return p.remove_filename().c_str();
}

//�^�ϊ�
std::string ToUTF8(const std::wstring& value)
{
    auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
    auto buffer = new char[length];

    WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

    std::string result(buffer);
    delete[] buffer;
    buffer = nullptr;

    return result;
}

// std::string(�}���`�o�C�g������)����std::wstring(���C�h������)�𓾂�
std::wstring ToWideString(const std::string& str)
{
    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

    std::wstring wstr;
    wstr.resize(num1);

    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

    assert(num1 == num2);
    return wstr;
}

bool AssimpLoader::Load(ImportSettings settings)
{
    if (settings.filename == nullptr)
    {
        return false;
    }

    auto& meshes = settings.meshes;
    auto inverseU = settings.inverseU;
    auto inverseV = settings.inverseV;

    auto path = ToUTF8(settings.filename);

    Assimp::Importer importer;
    int flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;

    auto scene = importer.ReadFile(path, flag);

    if (scene == nullptr)
    {
        // �����ǂݍ��݃G���[���ł���\������
        printf(importer.GetErrorString());
        printf("\n");
        return false;
    }

    // �ǂݍ��񂾃f�[�^�������Œ�`����Mesh�\���̂ɕϊ�����
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

void AssimpLoader::LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV)
{
    aiVector3D zero3D(0.0f, 0.0f, 0.0f);
    aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

    dst.vertices.resize(src->mNumVertices);

    for (auto i = 0u; i < src->mNumVertices; ++i)
    {
        auto position = &(src->mVertices[i]);
        auto normal = &(src->mNormals[i]);
        auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
        auto tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
        auto color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;

        // ���]�I�v�V��������������UV�𔽓]������
        if (inverseU)
        {
            uv->x = 1 - uv->x;
        }
        if (inverseV)
        {
            uv->y = 1 - uv->y;
        }

        NVertexAssimp vertex = {};
        vertex.pos = NVector3(position->x, position->y, position->z);
        vertex.normal = NVector3(normal->x, normal->y, normal->z);
        vertex.uv = NVector2(uv->x, uv->y);
        vertex.tangent = NVector3(tangent->x, tangent->y, tangent->z);
        vertex.color = NColor(color->r, color->g, color->b, color->a);

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

    indexBuff.Init(dst.indices);
    vertexBuff.Init(dst.vertices);
}

void AssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src)
{
    aiString path;
    if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
    {
        // �e�N�X�`���p�X�͑��΃p�X�œ����Ă���̂ŁA�t�@�C���̏ꏊ�Ƃ�������
        auto dir = GetDirectoryPath(filename);
        auto file = std::string(path.C_Str());
        dst.textureName = dir + ToWideString(file);
    }
    else
    {
        dst.textureName.clear();
    }
}