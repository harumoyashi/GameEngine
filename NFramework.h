#pragma once
class NFramework
{
private:
	//�Q�[���I���t���O
	bool isGameEnd_ = false;

public:
	virtual ~NFramework() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw() = 0;
	virtual void Finalize();

	//���s
	void Run();

	//�Q�[���I���t���O�擾
	virtual bool GetIsGameEnd()const { return isGameEnd_; }
	//�Q�[���I���t���O�ݒ�
	virtual void SetIsGameEnd(bool flag) { isGameEnd_ = flag; }
};