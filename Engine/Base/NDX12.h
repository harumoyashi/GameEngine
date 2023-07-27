#pragma once
#include "NWindows.h"
#include <DirectXTex.h>

#include <d3d12.h>
#include <dxgi1_6.h>	//Visual Studio Graphics Debugger���듮��N��������A1_4�ɂ���Ɖ������邩��

#include <wrl.h>

#include <DirectXMath.h>
#include <chrono>

#include "NInput.h"

class NDX12 final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//�f�o�b�O���C���[���I���ɂ��邽�߂Ɏg�p�����C���^�[�t�F�C�X
	ComPtr<ID3D12Debug> debugController_;

	//��{�I�u�W�F�N�g�̐���
	//�󂯎M�ɂȂ�ϐ�����
	ComPtr<ID3D12Device> device_;						//�F�X�Ȃ��̍쐬���邽�߂̉��z�A�_�v�^	
	ComPtr<IDXGIFactory6> dxgiFactory_;					//�A�_�v�^�[�̗񋓏�Ԃ̕ω������o�ł���悤�ɂ��邽�߂̃C���^�[�t�F�[�X	
	ComPtr<IDXGISwapChain4> swapchain_;					//�_�u���o�b�t�@�����O�̂��߂ɉ�ʐ؂�ւ��p�̃o�b�t�@�[�Ǘ�������	
	ComPtr<ID3D12CommandAllocator> commandAllocator_;	//�R�}���h���X�g�Ɋi�[���閽�߂ׂ̈̃��������Ǘ�����I�u�W�F�N�g	
	ComPtr<ID3D12GraphicsCommandList> commandList_;		//���߂��ꎞ�I�Ɋi�[���Ƃ����
	ComPtr<ID3D12CommandQueue> commandQueue_;			//GPU�����s����ׂ����߂̃��X�g��,�w�肵��������GPU�ɓ]�����邽�߂̃C���^�[�t�F�[�X

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	std::vector< ComPtr<IDXGIAdapter4>> adapters_;	//�A�_�v�^�[�̗񋓗p
	ComPtr<IDXGIAdapter4> tmpAdapter_;				//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������

	D3D_FEATURE_LEVEL featureLevel_;

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc_{};

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_{};

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;		//�����_�[�^�[�Q�b�g�r���[�n���h��
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�

	D3D12_RESOURCE_DESC depthResourceDesc_{};	//�[�x�o�b�t�@���\�[�X
	D3D12_HEAP_PROPERTIES depthHeapProp_{};		//�q�[�v�v���p�e�B
	D3D12_CLEAR_VALUE depthClearValue_{};
	ComPtr<ID3D12Resource> depthBuff_;			//�[�x�o�b�t�@
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc_{};	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ = {};

	ComPtr<ID3D12Fence> fence_;	//CPU��GPU�̓����Ɏg������
	UINT64 fenceVal_ = 0;

	uint32_t bbIndex_;
	D3D12_RESOURCE_BARRIER barrierDesc_{};		//���\�[�X�o���A
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};	//�f�X�N���v�^�n���h��

	D3D12_VIEWPORT viewport_{};	//�r���[�|�[�g
	D3D12_RECT scissorRect_{};	//�V�U�[��`

	//FPS�Œ�p
	float deltaTime_;				//1F�ł̌o�ߎ���
	std::chrono::steady_clock::time_point reference_;	//�L�^����
	const float kFPS = 60.0f;	//60FPS

public:
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

public:
	//�V���O���g���C���X�^���X�擾
	static NDX12* GetInstance();

	//DirectX������
	void Init(NWindows* win);
	// �`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//FPS�Œ�X�V
	//divideFrameRate:�t���[�����[�g��������1�ɂ��邩
	void UpdateFixFPX(const float divideFrameRate = 1.0f);

	//�Q�b�^�[//
	//�f�o�C�X�擾
	ID3D12Device* GetDevice()const { return device_.Get(); }
	//�X���b�v�`�F�[���擾
	IDXGISwapChain4* GetSwapchain()const { return swapchain_.Get(); }
	//�R�}���h�A���P�[�^�[�擾
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_.Get(); }
	//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	//�R�}���h�L���[�擾
	ID3D12CommandQueue* GetCommandQueue()const { return commandQueue_.Get(); }
	//RTV�q�[�v�擾
	ID3D12DescriptorHeap* GetRTVHeap()const { return rtvHeap_.Get(); }
	//SRV�q�[�v�擾
	ID3D12DescriptorHeap* GetSRVHeap()const { return srvHeap_.Get(); }
	//RTV�q�[�v�f�X�N�擾
	D3D12_DESCRIPTOR_HEAP_DESC GetRTVHeapDesc()const { return rtvHeapDesc_; }
	//DSV�q�[�v�擾
	ID3D12DescriptorHeap* GetDSVHeap()const { return dsvHeap_.Get(); }
	//�t�F���X�擾
	ID3D12Fence* GetFence()const { return fence_.Get(); }

	//�f���^�^�C���擾
	float GetDelta()const { return deltaTime_; }

private:
	//�A�_�v�^�[�I��
	void ChoiceAdapters();
	//�f�o�C�X�̐���
	void CreateDevice();
	//�R�}���h�A���P�[�^�A���X�g�A�L���[�̐���
	void CreateCommandGroup();
	// �X���b�v�`�F�[���̐���
	void CreateSwapChain(NWindows* win);
	//�V�F�[�_���\�[�X�r���[�̃f�X�N���v�^�q�[�v����
	void CreateSRVHeapDesc();
	//�����_�[�^�[�Q�b�g�r���[�̃f�X�N���v�^�q�[�v����
	void CreateRTVHeapDesc();
	//�����_�[�^�[�Q�b�g�r���[�̐���
	void CreateRTV();
	//�[�x�o�b�t�@�̃��\�[�X�ݒ�
	void SetDepthRes();
	//�[�x�o�b�t�@����
	void CreateDepthBuff();
	//�[�x�r���[�p�f�X�N���v�^�q�[�v����
	void CreateDescHeap();
	//�[�x�r���[�쐬
	void CreateDSV();
	//�t�F���X�̐���
	void CreateFence();

	//FPS�Œ菉����
	void InitializeFixFPS();

	//�o���A����
	void BarrierReset();
	//���߂̃N���[�Y
	//�����R�}���h���X�g�ɐςނ̂����܂�
	void CmdListClose();
	// �R�}���h���X�g�̎��s
	void ExecuteCmdList();
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	void BufferSwap();
	// �R�}���h�̎��s������҂�
	void CommandWait();
	// �L���[���N���A
	void ClearQueue();
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	void CmdListReset();

	// -------------------------PreDraw---------------------------- //
	//���\�[�X�o���A�ŏ������݉\�ɕύX
	void SetResBarrier();
	//�`���̕ύX
	void SetRenderTarget();
	//��ʃN���ARGBA
	void ClearScreen();
	//�r���[�|�[�g�ݒ�
	void SetViewport();
	//�V�U�[��`�ݒ�
	void SetScissorRect();

	private:
		NDX12() = default;
		NDX12(const NDX12&) = delete;
		~NDX12() = default;
		NDX12& operator=(const NDX12&) = delete;
};

