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
	NIndexBuff(uint32_t* list, uint32_t size);
	//�C���f�b�N�X�̔z���vector�Œ��_�o�b�t�@�����
	NIndexBuff(std::vector<uint32_t> list);

	//�C���f�b�N�X�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(uint32_t* list, uint32_t size);
	//�C���f�b�N�X�̔z���vector�Œ��_�o�b�t�@�����
	void Init(std::vector<uint32_t> list);

	ComPtr<ID3D12Resource> buff = nullptr;
	D3D12_INDEX_BUFFER_VIEW view{};
};