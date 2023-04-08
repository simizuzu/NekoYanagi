#include "GameScene.h"
#include "AvoidWarning.h"

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::Initalize()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	camera_->Update();
}

void GameScene::Draw()
{
}

void GameScene::Finalize()
{
}
