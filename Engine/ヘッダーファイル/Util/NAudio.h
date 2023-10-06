#pragma once
#include <xAudio2.h>

#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>
#include <fstream>
#include <wrl.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

//音声データ
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex{};
	//バッファの先頭アドレス
	std::vector<BYTE> pBuffer{};
	//バッファのサイズ
	uint32_t bufferSize{};
	//名前
	std::string name{};
};

// 再生データ
struct Voice {
	uint32_t handle = 0u;
	IXAudio2SourceVoice* sourceVoice = nullptr;
};

class NAudio final
{
private:
	// サウンドデータの最大数
	static const uint32_t kMaxSoundData_ = 256;

	//チャンクヘッダー
	struct ChunkHeader
	{
		char id[4];		//チャンク毎のID
		int32_t size;	//チャンクサイズ
	};

	//RIFFヘッダーチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];		//"WAVE"
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;	//"fmt"
		WAVEFORMATEX fmt;	//波形フォーマット
	};

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

	std::ifstream file_;			//ファイル入力ストリームのインスタンス
	FormatChunk format_ = {};	//フォーマットチャンク
	ChunkHeader data_;			//データチャンク
	char* pBuffer_;
	SoundData soundData_ = {};	//音声データ

	// MediaFoundation
	ComPtr<IMFSourceReader> mFSourceReader_;
	ComPtr<IMFMediaType> mFMediaType_;

	//オーディオコールバック
	class XAudio2VoiceCallback final
		: public IXAudio2VoiceCallback {
	public:
#pragma warning(push)
		//C4100の警告を見なかったことにする
#pragma warning(disable:4100)
		// ボイス処理パスの開始時
		STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ uint32_t BytesRequired) {};
		// ボイス処理パスの終了時
		STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
		// バッファストリームの再生が終了した時
		STDMETHOD_(void, OnStreamEnd)(THIS) {};
		// バッファの使用開始時
		STDMETHOD_(void, OnBufferStart)(THIS_ void* pBuffer_Context) {};
		// バッファの末尾に達した時
		STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBuffer_Context);
		// 再生がループ位置に達した時
		STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBuffer_Context) {};
		// ボイスの実行エラー時
		STDMETHOD_(void, OnVoiceError)(THIS_ void* pBuffer_Context, HRESULT Error) {};
#pragma warning(pop)
	};

public:
	static NAudio* GetInstance();

	//初期化
	//ディレクトリパスを指定しないと"Resources/sound/"になる
	void Init(const std::string& directoryPath = "Resources/sound/");

	//終了処理
	void Finalize();
	//WAV音声読み込み
	//"filename" = WAVファイル名
	//return サウンドデータハンドル
	uint32_t LoadWave(const std::string& filename);

	//mp3音声読み込み
	//"filename" = mp3ファイル名
	//return サウンドデータハンドル
	uint32_t LoadMP3(const std::string& filename);

	//サウンドデータの解放
	void Unload(SoundData* soundData);

	//音声再生
	//"soundDataHandle" サウンドデータハンドル
	//"loopFlag" ループ再生フラグ
	//"volume" ボリューム
	//"roopNum" ループする回数。0だとループはせず一度だけ流れる
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	//return 再生ハンドル
	uint32_t PlayWave(const uint32_t soundDataHandle, bool loopFlag = false, const float volume = 1.0f, const int roopNum = 0);

	//音声削除
	//"voiceHandle" 再生ハンドル
	void DestroyWave(const uint32_t voiceHandle);

	//音声一時停止
	//"voiceHandle" 再生ハンドル
	void StopWave(const uint32_t voiceHandle);

	//一時停止した音声を再生
	//"voiceHandle" 再生ハンドル
	void StartWave(const uint32_t voiceHandle);

	//音声再生中かどうか
	//"voiceHandle" 再生ハンドル
	bool IsPlaying(const uint32_t voiceHandle);

	//音量設定
	//"voiceHandle" 再生ハンドル
	//"volume" ボリューム
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	void SetVolume(const uint32_t voiceHandle, const float volume);

private:
	// サウンドデータコンテナ
	std::array<SoundData, kMaxSoundData_> soundDatas_;
	// 再生中データコンテナ
	std::set<Voice*> voices_;
	// サウンド格納ディレクトリ
	std::string directoryPath_;
	// 次に使うサウンドデータの番号
	uint32_t indexSoundData_ = 0u;
	// 次に使う再生中データの番号
	uint32_t indexVoice_ = 0u;
	// オーディオコールバック
	XAudio2VoiceCallback voiceCallback_;
};