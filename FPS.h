#pragma once
#include "AvoidWarning.h"

class FPS
{
public: // �����o�ϐ�
	// FPS�Œ菉����
	void InitializeFixFps();
	// FPS�Œ�X�V
	void UpdateFixFPS();

	// �L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;
};