#pragma once
#pragma warning(push)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 5262)
#pragma warning(disable: 5264)

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <windows.h>
#include <d3d12.h>
#include <vector>
#include <dxgi1_6.h>
#include <cassert>
#include <cstdint>
#include <array>
#include <DirectXMath.h>
#include <dinput.h>
#include <string>
#include <wrl.h>
#include <chrono>
#include <thread>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#pragma warning(pop)