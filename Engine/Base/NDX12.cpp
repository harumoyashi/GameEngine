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
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		debugController_->EnableDebugLayer();
	}
#endif
	InitializeFixFPS();

	// DXGIファクトリーの生成
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
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (uint32_t i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter_)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters_.push_back(tmpAdapter_);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters_.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters_[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter_ = adapters_[i];
			break;
		}
	}
}

void NDX12::CreateDevice()
{
	HRESULT result;

	// 対応レベルの配列
	std::vector<D3D_FEATURE_LEVEL> levels = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	//生成可能なバージョンが見つかったら打ち切り
	for (size_t i = 0; i < levels.size(); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter_.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result == S_OK) {	//HRESULT型の関数はtrue,falseの代わりにS_OK,特定の型が返される
			// デバイスを生成できた時点でループを抜ける
			featureLevel_ = levels[i];
			break;
		}
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//やばいエラーの時止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		//エラーの時止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	//警告の時止まる
	}

	//抑制するエラー
	std::vector<D3D12_MESSAGE_ID> denyIds = {
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//抑制される表示レベル
	std::vector<D3D12_MESSAGE_SEVERITY> severities = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = (uint32_t)denyIds.size();
	filter.DenyList.pIDList = denyIds.data();
	filter.DenyList.NumSeverities = (uint32_t)severities.size();
	filter.DenyList.pSeverityList = severities.data();
	//指定したエラーの表示を抑制する
	infoQueue->PushStorageFilter(&filter);
#endif // DEBUG
}

void NDX12::CreateCommandGroup()
{
	HRESULT result;

	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));

	//コマンドキューを生成
	result = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
}

void NDX12::CreateSwapChain(NWindows* win)
{
	HRESULT result;

	swapchainDesc_.Width = NWindows::kWin_width;
	swapchainDesc_.Height = NWindows::kWin_height;
	swapchainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapchainDesc_.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc_.BufferCount = 2; // バッファ数を2つに設定
	swapchainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//一時的なComPtr
	ComPtr<IDXGISwapChain1> swapchain_1;

	// スワップチェーンの生成
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), win->GetHwnd(), &swapchainDesc_, nullptr, nullptr, &swapchain_1);
	assert(SUCCEEDED(result));

	//もとのスワップチェーンに変換
	swapchain_1.As(&swapchain_);
}

void NDX12::CreateSRVHeapDesc()
{
	HRESULT result;

	//シェーダリソースビュー(SRV)の最大個数
	const size_t kMaxSRVCount = 2056;
	// デスクリプタヒープの設定
	srvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//シェーダから見えるように
	srvHeapDesc_.NumDescriptors = kMaxSRVCount;	//デスクリプタの持てる数設定

	//設定をもとにSRV用デスクリプタヒープを生成
	result = device_->CreateDescriptorHeap(&srvHeapDesc_, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));
}

void NDX12::CreateRTVHeapDesc()
{
	// デスクリプタヒープの設定
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //レンダーターゲットビュー
	rtvHeapDesc_.NumDescriptors = swapchainDesc_.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));
}

void NDX12::CreateRTV()
{
	// バックバッファ
	backBuffers_.resize(swapchainDesc_.BufferCount);

	// スワップチェーンの全てのバッファについて処理する
	for (uint32_t i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		swapchain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		// デスクリプタヒープのハンドルを取得
		rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc_, rtvHandle_);
	}
}

void NDX12::SetDepthRes()
{
	//リソース設定
	depthResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc_.Width = NWindows::kWin_width;	//レンダーターゲットに合わせる
	depthResourceDesc_.Height = NWindows::kWin_height;	//レンダーターゲットに合わせる
	depthResourceDesc_.DepthOrArraySize = 1;
	depthResourceDesc_.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc_.SampleDesc.Count = 1;
	depthResourceDesc_.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	//深度値用ヒーププロパティ
	depthHeapProp_.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	depthClearValue_.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue_.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
}

void NDX12::CreateDepthBuff()
{
	HRESULT result;

	result = device_->CreateCommittedResource(
		&depthHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc_,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue_,
		IID_PPV_ARGS(&depthBuff_)
	);
}

void NDX12::CreateDescHeap()
{
	HRESULT result;

	dsvHeapDesc_.NumDescriptors = 1;	//深度ビューは1つ
	dsvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー

	result = device_->CreateDescriptorHeap(&dsvHeapDesc_, IID_PPV_ARGS(&dsvHeap_));
}

void NDX12::CreateDSV()
{
	dsvDesc_.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
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
	//現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void NDX12::UpdateFixFPX(const float divideFrameRate)
{
	//1/60秒(1フレーム)ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / (kFPS / divideFrameRate)));
	//1/60秒よりちょっとだけ短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / (kFPS + 5.0f / divideFrameRate)));

	//デルタタイム記録
	deltaTime_ = (1000000.0f / (kFPS / divideFrameRate));

	//現在の時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回の記録からの経過時間を取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60秒(よりちょっとだけ短い時間)経ってない場合
	if (elapsed<kMinCheckTime)
	{
		//1/60秒経過するまで細かいスリープを繰り返す
		while (std::chrono::steady_clock::now()-reference_<kMinTime)
		{
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void NDX12::BarrierReset(D3D12_RESOURCE_BARRIER& barrierDesc)
{
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
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