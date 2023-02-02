#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

class NIndexBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NIndexBuff() {};	//����̃R���X�g���N�^

	//�C���f�b�N�X�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	NIndexBuff(unsigned int* list, unsigned int size);
	//�C���f�b�N�X�̔z���vector�Œ��_�o�b�t�@�����
	NIndexBuff(std::vector<unsigned int> list);

	//�C���f�b�N�X�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(unsigned int* list, unsigned int size);
	//�C���f�b�N�X�̔z���vector�Œ��_�o�b�t�@�����
	void Init(std::vector<unsigned int> list);

	ComPtr<ID3D12Resource> buff = nullptr;
	D3D12_INDEX_BUFFER_VIEW view{};
};