#include "WinApp.h"
#include "resouce.h"
#include "Input.h"

#pragma warning(push)
#include <imgui_impl_win32.h>
#pragma warning(pop)

#pragma comment(lib,"winmm.lib")

WinApp::WinApp() {}
WinApp::~WinApp() {}

WinApp* WinApp::winApp_ = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: // �E�B���h�E�j��
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
}

void WinApp::Initialize()
{
	//�E�B���h�E�N���X�ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"Engine";				// �E�B���h�E�N���X���i�w�b�_�[�Ŗ��O�ύX�\�j
	w.hInstance = GetModuleHandle(nullptr);		// �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	// �J�[�\���ݒ�
	w.hIcon = LoadIcon(GetModuleHandle(nullptr), TEXT("ICO"));

	// �E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0,0,window_width,window_height };
	// �����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(
		w.lpszClassName,		// �N���X��
		L"Engine",				// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �W���I�ȃE�B���h�E�X�^�C���i�w�b�_�[�ŕύX�\�j
		CW_USEDEFAULT,			// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,			// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,	// �E�B���h�E����
		wrc.bottom - wrc.top,	// �E�B���h�E�c��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		w.hInstance,			// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);				// �ǉ��p�����[�^

	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);

	// �V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);
}

bool WinApp::ProccessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return true;
	}
	else if (Input::GetInstance()->TriggerPushKey(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}

void WinApp::Finalize()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}

void WinApp::Delete()
{
	delete winApp_;
}

WinApp* WinApp::GetInstance()
{
	static WinApp winApp;

	return &winApp;
}

HWND WinApp::GetHwnd()
{
	return hwnd;
}

HINSTANCE WinApp::GetHinstance()
{
	return w.hInstance;
}
