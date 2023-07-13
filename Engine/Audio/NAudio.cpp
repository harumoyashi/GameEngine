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
	// �Đ����X�g���珜�O
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

	// XNAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result));

	//Media Foundation�̏�����
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	indexSoundData_ = 0u;
	indexVoice_ = 0u;
}

void NAudio::Finalize() {
	// XAudio2���
	xAudio2_.Reset();
	//Media Foundation���Ƃ�
	MFShutdown();
	// �����f�[�^���
	for (auto& soundData_ : soundDatas_) {
		Unload(&soundData_);
	}
}

uint32_t NAudio::LoadWave(const std::string& filename) {
	assert(indexSoundData_ < kMaxSoundData_);
	uint32_t handle = indexSoundData_;
	// �ǂݍ��ݍς݃T�E���h�f�[�^������
	auto it = std::find_if(soundDatas_.begin(), soundDatas_.end(), [&](const auto& soundData_) {
		return soundData_.name == filename;
		});
	if (it != soundDatas_.end()) {
		// �ǂݍ��ݍς݃T�E���h�f�[�^�̗v�f�ԍ����擾
		handle = static_cast<uint32_t>(std::distance(soundDatas_.begin(), it));
		return handle;
	}

	// �f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	bool currentRelative = false;
	if (2 < filename.size()) {
		currentRelative = (filename[0] == '.') && (filename[1] == '/');
	}
	std::string fullpath = currentRelative ? filename : directoryPath_ + filename;

	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fullpath, std::ios_base::binary);
	// �t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Format�`�����N�̓ǂݍ���
	// �`�����N�w�b�_�[�̊m�F
	file.read((char*)&format_, sizeof(ChunkHeader));
	if (strncmp(format_.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// �`�����N�{�̂̓ǂݍ���
	assert(format_.chunk.size <= sizeof(format_.fmt));
	file.read((char*)&format_.fmt, format_.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNK�`�����N�� Broadcast Wave Format�����o�����ꍇ�B
	while (_strnicmp(data.id, "junk", 4) == 0 || _strnicmp(data.id, "bext", 4) == 0 ||
		_strnicmp(data.id, "LIST", 4) == 0 || _strnicmp(data.id, "FLLR", 4) == 0) {
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		// �ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	if (_strnicmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	std::vector<BYTE> pBuffer(data.size);
	file.read((char*)pBuffer.data(), data.size);

	// Wave�t�@�C�������
	file.close();

	// �������ރT�E���h�f�[�^�̎Q��
	SoundData& soundData = soundDatas_.at(handle);

	soundData.wfex = format_.fmt;
	soundData.pBuffer = pBuffer;
	soundData.bufferSize = data.size;
	soundData.name = filename;

	//�����I�Ƀn���h�������ɐi�߂�
	indexSoundData_++;

	return handle;
}

uint32_t NAudio::LoadMP3(const std::string& filename)
{
	assert(indexSoundData_ < kMaxSoundData_);
	uint32_t handle = indexSoundData_;
	// �ǂݍ��ݍς݃T�E���h�f�[�^������
	auto it = std::find_if(soundDatas_.begin(), soundDatas_.end(), [&](const auto& soundData_) {
		return soundData_.name == filename;
		});
	if (it != soundDatas_.end()) {
		// �ǂݍ��ݍς݃T�E���h�f�[�^�̗v�f�ԍ����擾
		handle = static_cast<uint32_t>(std::distance(soundDatas_.begin(), it));
		return handle;
	}

	// �f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	bool currentRelative = false;
	if (2 < filename.size()) {
		currentRelative = (filename[0] == '.') && (filename[1] == '/');
	}
	std::string fullpath = currentRelative ? filename : directoryPath_ + filename;
	std::wstring wPath{ fullpath.begin(),fullpath.end() };

	// �\�[�X���[�_�[�̐���
	HRESULT result = MFCreateSourceReaderFromURL(wPath.c_str(), NULL, mFSourceReader_.GetAddressOf());
	assert(SUCCEEDED(result));

	// ���f�B�A�^�C�v�̎擾
	MFCreateMediaType(mFMediaType_.GetAddressOf());
	mFMediaType_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	mFMediaType_->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	mFSourceReader_->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, mFMediaType_.Get());

	mFMediaType_.Reset();
	mFSourceReader_->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, mFMediaType_.GetAddressOf());

	// �I�[�f�B�I�f�[�^�`���̍쐬
	WAVEFORMATEX* wfex{};
	MFCreateWaveFormatExFromMFMediaType(mFMediaType_.Get(), &wfex, nullptr);

	// �f�[�^�̓ǂݍ���
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

	// �������ރT�E���h�f�[�^�̎Q��
	SoundData& soundData = soundDatas_.at(handle);
	soundData = tempSound;

	//�����I�Ƀn���h�������ɐi�߂�
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

	// �T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = soundDatas_.at(soundDataHandle);
	// ���ǂݍ��݂̌��o
	assert(soundData.bufferSize != 0);

	uint32_t handle_ = soundDataHandle;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, &voiceCallback_);
	assert(SUCCEEDED(result));

	// �Đ����f�[�^
	Voice* voice = new Voice();
	voice->handle = soundDataHandle;
	voice->sourceVoice = pSourceVoice;
	// �Đ����f�[�^�R���e�i�ɓo�^
	voices_.insert(voice);

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = reinterpret_cast<BYTE*>(soundData.pBuffer.data());
	buf.pContext = voice;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) {
		// �������[�v
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		buf.LoopCount = roopNum;
	}

	// �g�`�f�[�^�̍Đ�
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
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) {
			return voice->handle == voiceHandle; });
	// ����
	if (it != voices_.end()) {
		(*it)->sourceVoice->DestroyVoice();

		voices_.erase(it);
	}
}

void NAudio::StopWave(const uint32_t voiceHandle) {

	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// ����
	if (it != voices_.end()) {
		(*it)->sourceVoice->Stop();
	}
}

void NAudio::StartWave(const uint32_t voiceHandle)
{
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// ����
	if (it != voices_.end()) {
		// �Đ��ĊJ
		(*it)->sourceVoice->Start();
	}
}

bool NAudio::IsPlaying(const uint32_t voiceHandle) {
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// �����B�Đ��I����Ă�̂��ǂ����𔻒f
	if (it != voices_.end()) {
		XAUDIO2_VOICE_STATE state{};
		(*it)->sourceVoice->GetState(&state);
		return state.SamplesPlayed != 0;
	}
	return false;
}

void NAudio::SetVolume(const uint32_t voiceHandle, const float volume) {
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// ����
	if (it != voices_.end()) {
		(*it)->sourceVoice->SetVolume(volume);
	}
}
