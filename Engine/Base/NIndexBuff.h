#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

struct NIndexBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> buff_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW view_{};

public:
	NIndexBuff() {};	//����̃R���X�g���N�^

	//�C���f�b�N�X�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	NIndexBuff(uint32_t* list, const uint32_t size);
	//�C���f�b�N�X�̔z���vector�Œ��_�o�b�t�@�����
	NIndexBuff(const std::vector<uint32_t>& list);

	//�C���f�b�N�X�̔z��Ƃ��̑傫���Œ��_�o�b�t�@�����
	void Init(uint32_t* list, const uint32_t size);
	//�C���f�b�N�X�̔z���vector�Œ��_�o�b�t�@�����
	void Init(const std::vector<uint32_t>& list);

	// �Q�b�^�[ //
	//�o�b�t�@�[�擾
	inline ID3D12Resource* GetBuffer() { return buff_.Get(); }
	//�r���[�擾
	inline D3D12_INDEX_BUFFER_VIEW* GetView() { return &view_; }
};