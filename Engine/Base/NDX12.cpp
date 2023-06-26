#include "NDX12.h"
#include <thread>

NDX12* NDX12::GetInstance()
{
	static NDX12 instance;
	return &instance;
}

void NDX12::Init(NWindows* win)
{
	HRESULT result;

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		debugController_->EnableDebugLayer();
	}
#endif
	InitializeFixFPS();

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	ChoiceAdapters();
	CreateDevice();
	CreateCommandGroup();
	CreateSwapChain(win);
	CreateSRVHeapDesc();
	CreateRTVHeapDesc();
	CreateRTV();
	SetDepthRes();
	CreateDepthBuff();
	CreateDescHeap();
	CreateDSV();
	CreateFence();
}

void NDX12::PostDraw(D3D12_RESOURCE_BARRIER& barrierDesc)
{
	BarrierReset(barrierDesc);
	CmdListClose();
	ExecuteCmdList();
	BufferSwap();
	CommandWait();
	UpdateFixFPX();
	ClearQueue();
	CmdListReset();
}

void NDX12::ChoiceAdapters()
{
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (uint32_t i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter_)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// ���I�z��ɒǉ�����
		adapters_.push_back(tmpAdapter_);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters_.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters_[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter_ = adapters_[i];
			break;
		}
	}
}

void NDX12::CreateDevice()
{
	HRESULT result;

	// �Ή����x���̔z��
	std::vector<D3D_FEATURE_LEVEL> levels = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	//�����\�ȃo�[�W����������������ł��؂�
	for (size_t i = 0; i < levels.size(); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter_.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result == S_OK) {	//HRESULT�^�̊֐���true,false�̑����S_OK,����̌^���Ԃ����
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel_ = levels[i];
			break;
		}
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//��΂��G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		//�G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	//�x���̎��~�܂�
	}

	//�}������G���[
	std::vector<D3D12_MESSAGE_ID> denyIds = {
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//�}�������\�����x��
	std::vector<D3D12_MESSAGE_SEVERITY> severities = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = (uint32_t)denyIds.size();
	filter.DenyList.pIDList = denyIds.data();
	filter.DenyList.NumSeverities = (uint32_t)severities.size();
	filter.DenyList.pSeverityList = severities.data();
	//�w�肵���G���[�̕\����}������
	infoQueue->PushStorageFilter(&filter);
#endif // DEBUG
}

void NDX12::CreateCommandGroup()
{
	HRESULT result;

	// �R�}���h�A���P�[�^�𐶐�
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�𐶐�
	result = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�𐶐�
	result = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
}

void NDX12::CreateSwapChain(NWindows* win)
{
	HRESULT result;

	swapchainDesc_.Width = NWindows::kWin_width;
	swapchainDesc_.Height = NWindows::kWin_height;
	swapchainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapchainDesc_.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapchainDesc_.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapchainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapchainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�ꎞ�I��ComPtr
	ComPtr<IDXGISwapChain1> swapchain_1;

	// �X���b�v�`�F�[���̐���
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), win->GetHwnd(), &swapchainDesc_, nullptr, nullptr, &swapchain_1);
	assert(SUCCEEDED(result));

	//���Ƃ̃X���b�v�`�F�[���ɕϊ�
	swapchain_1.As(&swapchain_);
}

void NDX12::CreateSRVHeapDesc()
{
	HRESULT result;

	//�V�F�[�_���\�[�X�r���[(SRV)�̍ő��
	const size_t kMaxSRVCount = 2056;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//�V�F�[�_���猩����悤��
	srvHeapDesc_.NumDescriptors = kMaxSRVCount;	//�f�X�N���v�^�̎��Ă鐔�ݒ�

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device_->CreateDescriptorHeap(&srvHeapDesc_, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));
}

void NDX12::CreateRTVHeapDesc()
{
	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //�����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc_.NumDescriptors = swapchainDesc_.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));
}

void NDX12::CreateRTV()
{
	// �o�b�N�o�b�t�@
	backBuffers_.resize(swapchainDesc_.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (uint32_t i = 0; i < backBuffers_.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapchain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc_, rtvHandle_);
	}
}

void NDX12::SetDepthRes()
{
	//���\�[�X�ݒ�
	depthResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc_.Width = NWindows::kWin_width;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc_.Height = NWindows::kWin_height;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc_.DepthOrArraySize = 1;
	depthResourceDesc_.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc_.SampleDesc.Count = 1;
	depthResourceDesc_.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	depthHeapProp_.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	depthClearValue_.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue_.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
}

void NDX12::CreateDepthBuff()
{
	HRESULT result;

	result = device_->CreateCommittedResource(
		&depthHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc_,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue_,
		IID_PPV_ARGS(&depthBuff_)
	);
}

void NDX12::CreateDescHeap()
{
	HRESULT result;

	dsvHeapDesc_.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[

	result = device_->CreateDescriptorHeap(&dsvHeapDesc_, IID_PPV_ARGS(&dsvHeap_));
}

void NDX12::CreateDSV()
{
	dsvDesc_.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc_, dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);
}

void NDX12::CreateFence()
{
	HRESULT result;

	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));
}

void NDX12::InitializeFixFPS()
{
	//���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void NDX12::UpdateFixFPX(const float divideFrameRate)
{
	//1/60�b(1�t���[��)�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / (kFPS / divideFrameRate)));
	//1/60�b��肿����Ƃ����Z������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / (kFPS + 5.0f / divideFrameRate)));

	//�f���^�^�C���L�^
	deltaTime_ = (1000000.0f / (kFPS / divideFrameRate));

	//���݂̎��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//�O��̋L�^����̌o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60�b(��肿����Ƃ����Z������)�o���ĂȂ��ꍇ
	if (elapsed<kMinCheckTime)
	{
		//1/60�b�o�߂���܂ōׂ����X���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now()-reference_<kMinTime)
		{
			//1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void NDX12::BarrierReset(D3D12_RESOURCE_BARRIER& barrierDesc)
{
	// 5.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//�\����Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc);
}

void NDX12::CmdListClose()
{
	HRESULT result;

	result = commandList_->Close();
	assert(SUCCEEDED(result));
}

void NDX12::ExecuteCmdList()
{
	std::vector<ID3D12CommandList*> commandLists = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, &commandLists[0]);
}

void NDX12::BufferSwap()
{
	HRESULT result;

	result = swapchain_->Present(1, 0);
	assert(SUCCEEDED(result));
}

void NDX12::CommandWait()
{
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

void NDX12::ClearQueue()
{
	HRESULT result;

	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
}

void NDX12::CmdListReset()
{
	HRESULT result;

	result = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));
}