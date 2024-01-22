#pragma once
#include "NWindows.h"
#include <DirectXTex.h>

#include <d3d12.h>
#include <dxgi1_6.h>	//Visual Studio Graphics Debuggerが誤動作起こしたら、1_4にすると解決するかも

#include <wrl.h>

#include <DirectXMath.h>
#include <chrono>

#include "NInput.h"

// ゲームに必要な基盤部分 //

class NDX12 final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//デバッグレイヤーをオンにするために使用されるインターフェイス
	ComPtr<ID3D12Debug1> debugController_;

	//基本オブジェクトの生成
	//受け皿になる変数生成
	ComPtr<ID3D12Device> device_;						//色々なもの作成するための仮想アダプタ	
	ComPtr<IDXGIFactory6> dxgiFactory_;					//アダプターの列挙状態の変化を検出できるようにするためのインターフェース	
	ComPtr<IDXGISwapChain4> swapchain_;					//ダブルバッファリングのために画面切り替え用のバッファー管理するやつ	
	ComPtr<ID3D12CommandAllocator> commandAllocator_;	//コマンドリストに格納する命令の為のメモリを管理するオブジェクト	
	ComPtr<ID3D12GraphicsCommandList> commandList_;		//命令を一時的に格納しとくやつ
	ComPtr<ID3D12CommandQueue> commandQueue_;			//GPUが実行するべき命令のリストを,指定した順序でGPUに転送するためのインターフェース

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	std::vector< ComPtr<IDXGIAdapter4>> adapters_;	//アダプターの列挙用
	ComPtr<IDXGIAdapter4> tmpAdapter_;				//ここに特定の名前を持つアダプターオブジェクトが入る

	D3D_FEATURE_LEVEL featureLevel_;

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc_{};

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_{};

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;		//レンダーターゲットビューハンドル
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};	//レンダーターゲットビューの設定

	D3D12_RESOURCE_DESC depthResourceDesc_{};	//深度バッファリソース
	D3D12_HEAP_PROPERTIES depthHeapProp_{};		//ヒーププロパティ
	D3D12_CLEAR_VALUE depthClearValue_{};
	ComPtr<ID3D12Resource> depthBuff_;			//深度バッファ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc_{};	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ = {};

	ComPtr<ID3D12Fence> fence_;	//CPUとGPUの同期に使われるやつ
	UINT64 fenceVal_ = 0;

	uint32_t bbIndex_;
	D3D12_RESOURCE_BARRIER barrierDesc_{};		//リソースバリア
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};	//デスクリプタハンドル

	D3D12_VIEWPORT viewport_{};	//ビューポート
	D3D12_RECT scissorRect_{};	//シザー矩形

	//FPS固定用
	float deltaTime_;				//1Fでの経過時間
	std::chrono::steady_clock::time_point reference_;	//記録時間
	const float kFPS = 60.0f;	//60FPS

public:
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

public:
	//シングルトンインスタンス取得
	static NDX12* GetInstance();

	//DirectX初期化
	void Init(NWindows* win);
	// 描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();

	//FPS固定更新
	//divideFrameRate:フレームレートを何分の1にするか
	void UpdateFixFPX(const float divideFrameRate = 1.0f);

	//ゲッター//
	//デバイス取得
	ID3D12Device* GetDevice()const { return device_.Get(); }
	//スワップチェーン取得
	IDXGISwapChain4* GetSwapchain()const { return swapchain_.Get(); }
	//コマンドアロケーター取得
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_.Get(); }
	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	//コマンドキュー取得
	ID3D12CommandQueue* GetCommandQueue()const { return commandQueue_.Get(); }
	//RTVヒープ取得
	ID3D12DescriptorHeap* GetRTVHeap()const { return rtvHeap_.Get(); }
	//SRVヒープ取得
	ID3D12DescriptorHeap* GetSRVHeap()const { return srvHeap_.Get(); }
	//RTVヒープデスク取得
	D3D12_DESCRIPTOR_HEAP_DESC GetRTVHeapDesc()const { return rtvHeapDesc_; }
	//DSVヒープ取得
	ID3D12DescriptorHeap* GetDSVHeap()const { return dsvHeap_.Get(); }
	//フェンス取得
	ID3D12Fence* GetFence()const { return fence_.Get(); }

	//デルタタイム取得
	float GetDelta()const { return deltaTime_; }

private:
	//アダプター選択
	void ChoiceAdapters();
	//デバイスの生成
	void CreateDevice();
	//コマンドアロケータ、リスト、キューの生成
	void CreateCommandGroup();
	// スワップチェーンの生成
	void CreateSwapChain(NWindows* win);
	//シェーダリソースビューのデスクリプタヒープ生成
	void CreateSRVHeapDesc();
	//レンダーターゲットビューのデスクリプタヒープ生成
	void CreateRTVHeapDesc();
	//レンダーターゲットビューの生成
	void CreateRTV();
	//深度バッファのリソース設定
	void SetDepthRes();
	//深度バッファ生成
	void CreateDepthBuff();
	//深度ビュー用デスクリプタヒープ生成
	void CreateDescHeap();
	//深度ビュー作成
	void CreateDSV();
	//フェンスの生成
	void CreateFence();

	//FPS固定初期化
	void InitializeFixFPS();

	//バリア解除
	void BarrierReset();
	//命令のクローズ
	//もうコマンドリストに積むのおしまい
	void CmdListClose();
	// コマンドリストの実行
	void ExecuteCmdList();
	// 画面に表示するバッファをフリップ(裏表の入替え)
	void BufferSwap();
	// コマンドの実行完了を待つ
	void CommandWait();
	// キューをクリア
	void ClearQueue();
	// 再びコマンドリストを貯める準備
	void CmdListReset();

	// -------------------------PreDraw---------------------------- //
	//リソースバリアで書き込み可能に変更
	void SetResBarrier();
	//描画先の変更
	void SetRenderTarget();
	//画面クリアRGBA
	void ClearScreen();
	//ビューポート設定
	void SetViewport();
	//シザー矩形設定
	void SetScissorRect();

private:
	//コンストラクタ
	NDX12() = default;
	//コピー禁止
	NDX12(const NDX12&) = delete;
	//デストラクタ
	~NDX12() = default;
	//コピー禁止
	NDX12& operator=(const NDX12&) = delete;
};

