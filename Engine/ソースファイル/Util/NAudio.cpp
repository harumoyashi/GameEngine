#include "NAudio.h"

#include <cassert>
#include <fstream>
#include <windows.h>

#pragma comment(lib, "xAudio2.lib")

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

void NAudio::XAudio2VoiceCallback::OnBufferEnd(THIS_ void* pBufferContext) {

	Voice* voice = reinterpret_cast<Voice*>(pBufferContext);
	// 再生リストから除外
	NAudio::GetInstance()->voices_.erase(voice);
}

NAudio* NAudio::GetInstance() {
	static NAudio instance;

	return &instance;
}

void NAudio::Init(const std::string& directoryPath) {
	directoryPath_ = directoryPath;

	HRESULT result;
	IXAudio2MasteringVoice* masterVoice_;

	// XNAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result));

	//Media Foundationの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	indexSoundData_ = 0u;
	indexVoice_ = 0u;
}

void NAudio::Finalize() {
	// XAudio2解放
	xAudio2_.Reset();
	//Media Foundation落とす
	MFShutdown();
	// 音声データ解放
	for (auto& soundData_ : soundDatas_) {
		Unload(&soundData_);
	}
}

uint32_t NAudio::LoadWave(const std::string& filename) {
	assert(indexSoundData_ < kMaxSoundData_);
	uint32_t handle = indexSoundData_;
	// 読み込み済みサウンドデータを検索
	auto it = std::find_if(soundDatas_.begin(), soundDatas_.end(), [&](const auto& soundData_) {
		return soundData_.name == filename;
		});
	if (it != soundDatas_.end()) {
		// 読み込み済みサウンドデータの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(soundDatas_.begin(), it));
		return handle;
	}

	// ディレクトリパスとファイル名を連結してフルパスを得る
	bool currentRelative = false;
	if (2 < filename.size()) {
		currentRelative = (filename[0] == '.') && (filename[1] == '/');
	}
	std::string fullpath = currentRelative ? filename : directoryPath_ + filename;

	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(fullpath, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	// チャンクヘッダーの確認
	file.read((char*)&format_, sizeof(ChunkHeader));
	if (strncmp(format_.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format_.chunk.size <= sizeof(format_.fmt));
	file.read((char*)&format_.fmt, format_.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクか Broadcast Wave Formatを検出した場合。
	while (_strnicmp(data.id, "junk", 4) == 0 || _strnicmp(data.id, "bext", 4) == 0 ||
		_strnicmp(data.id, "LIST", 4) == 0 || _strnicmp(data.id, "FLLR", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (_strnicmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	std::vector<BYTE> pBuffer(data.size);
	file.read((char*)pBuffer.data(), data.size);

	// Waveファイルを閉じる
	file.close();

	// 書き込むサウンドデータの参照
	SoundData& soundData = soundDatas_.at(handle);

	soundData.wfex = format_.fmt;
	soundData.pBuffer = pBuffer;
	soundData.bufferSize = data.size;
	soundData.name = filename;

	//実質的にハンドルを次に進める
	indexSoundData_++;

	return handle;
}

uint32_t NAudio::LoadMP3(const std::string& filename)
{
	assert(indexSoundData_ < kMaxSoundData_);
	uint32_t handle = indexSoundData_;
	// 読み込み済みサウンドデータを検索
	auto it = std::find_if(soundDatas_.begin(), soundDatas_.end(), [&](const auto& soundData_) {
		return soundData_.name == filename;
		});
	if (it != soundDatas_.end()) {
		// 読み込み済みサウンドデータの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(soundDatas_.begin(), it));
		return handle;
	}

	// ディレクトリパスとファイル名を連結してフルパスを得る
	bool currentRelative = false;
	if (2 < filename.size()) {
		currentRelative = (filename[0] == '.') && (filename[1] == '/');
	}
	std::string fullpath = currentRelative ? filename : directoryPath_ + filename;
	std::wstring wPath{ fullpath.begin(),fullpath.end() };

	// ソースリーダーの生成
	HRESULT result = MFCreateSourceReaderFromURL(wPath.c_str(), NULL, mFSourceReader_.GetAddressOf());
	assert(SUCCEEDED(result));

	// メディアタイプの取得
	MFCreateMediaType(mFMediaType_.GetAddressOf());
	mFMediaType_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	mFMediaType_->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	mFSourceReader_->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, mFMediaType_.Get());

	mFMediaType_.Reset();
	mFSourceReader_->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, mFMediaType_.GetAddressOf());

	// オーディオデータ形式の作成
	WAVEFORMATEX* wfex{};
	MFCreateWaveFormatExFromMFMediaType(mFMediaType_.Get(), &wfex, nullptr);

	// データの読み込み
	std::vector<BYTE> mediaData;
	while (true)
	{
		IMFSample* pMFSample{};
		DWORD dwStreamFlags{};
		mFSourceReader_->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) break;

		IMFMediaBuffer* pMFMediaBuffer{};
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{};
		DWORD cbCurrentLength{};
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		mediaData.resize(mediaData.size() + cbCurrentLength);
		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}
	SoundData tempSound{ *wfex, mediaData, sizeof(BYTE) * static_cast<uint32_t>(mediaData.size()) };

	CoTaskMemFree(wfex);
	mFSourceReader_.Reset();
	mFMediaType_.Reset();

	// 書き込むサウンドデータの参照
	SoundData& soundData = soundDatas_.at(handle);
	soundData = tempSound;

	//実質的にハンドルを次に進める
	indexSoundData_++;

	return handle;
}

void NAudio::Unload(SoundData* soundData) {
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

uint32_t NAudio::PlayWave(const uint32_t soundDataHandle, bool loopFlag, const float volume, const int roopNum) {
	HRESULT result;
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	assert(soundDataHandle <= soundDatas_.size());

	// サウンドデータの参照を取得
	SoundData& soundData = soundDatas_.at(soundDataHandle);
	// 未読み込みの検出
	assert(soundData.bufferSize != 0);

	uint32_t handle_ = soundDataHandle;

	// 波形フォーマットを元にSourceVoiceの生成
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, &voiceCallback_);
	assert(SUCCEEDED(result));

	// 再生中データ
	Voice* voice = new Voice();
	voice->handle = soundDataHandle;
	voice->sourceVoice = pSourceVoice;
	// 再生中データコンテナに登録
	voices_.insert(voice);

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = reinterpret_cast<BYTE*>(soundData.pBuffer.data());
	buf.pContext = voice;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) {
		// 無限ループ
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		buf.LoopCount = roopNum;
	}

	// 波形データの再生
	if (buf.LoopCount >= 0)
	{
		result = pSourceVoice->SubmitSourceBuffer(&buf);
		pSourceVoice->SetVolume(volume);
		result = pSourceVoice->Start();
	}
	buf.LoopCount--;

	return handle_;
}

void NAudio::DestroyWave(const uint32_t voiceHandle)
{
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) {
			return voice->handle == voiceHandle; });
	// 発見
	if (it != voices_.end()) {
		(*it)->sourceVoice->DestroyVoice();

		voices_.erase(it);
	}
}

void NAudio::StopWave(const uint32_t voiceHandle) {

	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見
	if (it != voices_.end()) {
		(*it)->sourceVoice->Stop();
	}
}

void NAudio::StartWave(const uint32_t voiceHandle)
{
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見
	if (it != voices_.end()) {
		// 再生再開
		(*it)->sourceVoice->Start();
	}
}

bool NAudio::IsPlaying(const uint32_t voiceHandle) {
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見。再生終わってるのかどうかを判断
	if (it != voices_.end()) {
		XAUDIO2_VOICE_STATE state{};
		(*it)->sourceVoice->GetState(&state);
		return state.SamplesPlayed != 0;
	}
	return false;
}

void NAudio::SetVolume(const uint32_t voiceHandle, const float volume) {
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見
	if (it != voices_.end()) {
		(*it)->sourceVoice->SetVolume(volume);
	}
}
