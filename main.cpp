#include "AvoidWarning.h"

#include "WinApp.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "FPS.h"
#include "GameScene.h"
#include "Input.h"
#include "ParticleManager.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	WinApp* winApp = WinApp::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();
	Input* input = Input::GetInstance();

	
	FPS* fps = new FPS();
	GameScene* gameScene = new GameScene();


	winApp->Initialize();
	fps->InitializeFixFps();
	dxCommon->Initialize();
	imGuiManager->Initialize(winApp, dxCommon);
	input->Initialize();




	ParticleManager::StaticInitialize(dxCommon->GetDevice(),dxCommon->GetCommandList());
	gameScene->Initalize();
	while (true)
	{
		//更新
		imGuiManager->Begin();

		// ゲームシーン //
		gameScene->Update();

		input->Update();

		imGuiManager->End();

		//描画
		if (winApp->ProccessMessage())
		{
			break;
		}
		dxCommon->PreDraw(winApp);
		imGuiManager->Draw(dxCommon);
		gameScene->Draw();
		dxCommon->PostDraw();
		fps->UpdateFixFPS();
	}

	imGuiManager->Finalize();
	dxCommon->Delete();
	winApp->Finalize();
	winApp->Delete();
	delete fps;
}