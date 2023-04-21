#include "GameScene.h"
#include "AvoidWarning.h"



GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::Initalize()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	particles = std::make_unique<ParticleManager>();
	particles->LoadTexture(L"Resources/effect.png");
	transform.Initialize();
}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	camera_->Update();
	camera_->SetTarget({1.0f,0.0f,0.0f});
	particles->RandParticle();
	particles->Update();

	transform.Update(camera_.get());
}

void GameScene::Draw()
{
	particles->Draw(&transform);
}

void GameScene::Finalize()
{
}
