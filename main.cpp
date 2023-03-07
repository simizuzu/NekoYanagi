#include "AvoidWarning.h"

#include "WinApp.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "FPS.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	WinApp* winApp = WinApp::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();


	FPS* fps = new FPS();


	winApp->Initialize();
	fps->InitializeFixFps();
	dxCommon->Initialize();
	imGuiManager->Initialize(winApp, dxCommon);

	while (true)
	{
		imGuiManager->Begin();

		// ƒQ[ƒ€ƒV[ƒ“ //

		imGuiManager->End();

		if (winApp->ProccessMessage())
		{
			break;
		}
		dxCommon->PreDraw(winApp);
		imGuiManager->Draw(dxCommon);
		dxCommon->PostDraw();
		fps->UpdateFixFPS();
	}

	imGuiManager->Finalize();
	dxCommon->Delete();
	winApp->Finalize();
	winApp->Delete();
	delete fps;
}