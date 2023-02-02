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
	NVertexBuff(NVertex* vertices, unsigned int size);
	//Vertex(Pos)��vector�Œ��_�o�b�t�@�����
	NVertexBuff(std::vector<NVertex> vertices);

	//Vertex(PosNormalUv)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	NVertexBuff(NVertexPNU* vertices, unsigned int size);
	//Vertex(PosNormalUv)��vector�Œ��_�o�b�t�@�����
	NVertexBuff(std::vector<NVertexPNU> vertices);

	//Vertex(Pos)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(NVertex* vertices, unsigned int size);
	//Vertex(Pos)��vector�Œ��_�o�b�t�@�����
	void Init(std::vector<NVertex> vertices);

	//Vertex(PosNormalUv)�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(NVertexPNU* vertices, unsigned int size);
	//Vertex(PosNormalUv)��vector�Œ��_�o�b�t�@�����
	void Init(std::vector<NVertexPNU> vertices);

	ComPtr<ID3D12Resource> buff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW view{};
};