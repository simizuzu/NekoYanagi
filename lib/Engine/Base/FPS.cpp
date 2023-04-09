#include "FPS.h"

void FPS::InitializeFixFps()
{
	// ���ݎ��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void FPS::UpdateFixFPS()
{
	// 1/60�b�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60�b���킸���ɒZ������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// ���ݎ��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// �O��L�^����̌o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60�b�i���킸���ɒZ�����ԁj�o���Ă��Ȃ��ꍇ
	if (elapsed < kMinCheckTime)
	{
		// 1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			// 1�}�C�N���b�X�R�[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// ���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}
