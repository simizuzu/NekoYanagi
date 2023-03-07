#pragma once
#include "AvoidWarning.h"

class WinApp
{
public: // �ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �萔
	// �E�B���h�E����
	static const int window_height = 720;
	// �E�B���h�E�c��
	static const int window_width = 1280;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	bool ProccessMessage();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �������
	/// </summary>
	void Delete();

	// �V���O���g��
	static WinApp* GetInstance();

public: // �Q�b�^�[
	HWND GetHwnd();
	HINSTANCE GetHinstance();

private: // �����o�ϐ�
	// �E�B���h�E�n���h��
	HWND hwnd = nullptr;
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	static WinApp* winApp_;

private:
	WinApp();
	~WinApp();
	WinApp& operator=(const WinApp&) = delete;
	WinApp(const WinApp&) = delete;
};