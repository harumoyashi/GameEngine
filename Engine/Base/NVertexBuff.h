#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "NVertex.h"

class NVertexBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NVertexBuff() {};	//����̃R���X�g���N�^

	//Vertex(Pos)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	NVertexBuff(NVertex* vertices_, uint32_t size);
	//Vertex(Pos)��vector�Œ��_�o�b�t�@�����
	NVertexBuff(std::vector<NVertex> vertices_);

	//Vertex(PosNormalUv)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	NVertexBuff(NVertexPNU* vertices_, uint32_t size);
	//Vertex(PosNormalUv)��vector�Œ��_�o�b�t�@�����
	NVertexBuff(std::vector<NVertexPNU> vertices_);

	//Vertex(NVertexAssimp)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	NVertexBuff(NVertexAssimp* vertices_, uint32_t size);
	//Vertex(NVertexAssimp)��vector�Œ��_�o�b�t�@�����
	NVertexBuff(std::vector<NVertexAssimp> vertices_);

	//Vertex(Pos)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(NVertex* vertices_, uint32_t size);
	//Vertex(Pos)��vector�Œ��_�o�b�t�@�����
	void Init(std::vector<NVertex> vertices_);

	//Vertex(PosNormalUv)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(NVertexPNU* vertices_, uint32_t size);
	//Vertex(PosNormalUv)��vector�Œ��_�o�b�t�@�����
	void Init(std::vector<NVertexPNU> vertices_);

	//Vertex(assimp�p)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(NVertexAssimp* vertices_, uint32_t size);
	//Vertex(assimp�p)��vector�Œ��_�o�b�t�@�����
	void Init(std::vector<NVertexAssimp> vertices_);

	ComPtr<ID3D12Resource> buff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW view{};
};