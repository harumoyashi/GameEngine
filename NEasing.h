#pragma once
namespace NEasing
{
	//�C�[�W���O�Q
	float lerp(float start, float end, float timeRate);

	float InQuad(float timeRate);
	float OutQuad(float timeRate);
	float InQuad(float start, float end, float timeRate);
	float OutQuad(float start, float end, float timeRate);

	float InOutQuad(float timeRate);
	float InOutQuad(float start, float end, float timeRate);

	float OutBounce(float timeRate);

	float OutBounce(float start, float end, float timeRate);
	float InBounce(float start, float end, float timeRate);
	float InOutBounce(float start, float end, float timeRate);

	float InElastic(float timeRate);
	float OutElastic(float timeRate);

	float InElastic(float start, float end, float timeRate);
	float OutElastic(float start, float end, float timeRate);

	float InBack(float timeRate);
	float InBack(float start, float end, float timeRate);

	float OutBack(float timeRate);
	float OutBack(float start, float end, float timeRate);

	float InOutBack(float timeRate);
	float InOutBack(float start, float end, float timeRate);

	//�C�[�W���O�p�̃^�C�}�[
	class easeTimer
	{
	private:
		//�i�s���t���O
		bool run = false;
		//�i�s�I���t���O
		bool end = false;
		//��ޒ��t���O
		bool reverse_ = false;
		//��ޏI���t���O
		bool reverseend_ = false;

	public:
		//�R���X�g���N�^
		easeTimer(float maxTime = 1.0f) {
			maxTime_ = maxTime;
		}

		float nowTime_ = 0.0f;		// ���݂̌o�ߎ���
		float maxTime_ = 1.0f;		// ���b�����Ĉړ����邩

		//�^�C�}�[���i��ł邩�t���O�擾
		inline bool GetRun()const { return run; };
		//��x�ł��i�s���ꂽ��true�ɂȂ�t���O�擾
		inline bool GetStarted()const { return run || end; };
		//�^�C�}�[���i�ݐ؂������t���O�擾
		inline bool GetEnd()const { return end; };
		//�i�ݐ؂������t���O��ݒ�
		inline void SetEnd(bool end) { this->end = end; };

		//�^�C�}�[���߂��Ă邩�t���O�擾
		inline bool GetReverse()const { return reverse_; };
		//�^�C�}�[���߂�؂������t���O�擾
		inline bool GetReverseEnd()const { return reverseend_; };
		//�߂�؂������t���O��ݒ�
		inline void SetReverseEnd(bool reverseend) { reverseend_ = reverseend; };
		//��x�ł���ނ��ꂽ��true�ɂȂ�t���O�擾
		inline bool GetReverseStarted()const { return reverse_ || reverseend_; };

		//�^�C�}�[�͋N�����Ȃ�������������
		void Reset();
		//�Ăяo������^�C�}�[���i�s
		void Start();
		//�Ăяo������^�C�}�[�����
		void ReverseStart();
		//�o�ߎ��Ԃ��X�V
		void Update();

		//���݂��ő�b���̂����ǂ̊�������(0.0f�`1.0f)�̒l�ŕԂ�
		float GetTimeRate();
	};
};