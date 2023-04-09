#pragma once
#include "Camera.h"
#include "ParticleManager.h"
#include "WorldTransform.h"

class GameScene
{
public: //�����o�ϐ�
	//�R���X�g���N�^�E�f�X�g���N�^
	GameScene();
	~GameScene();
	//������
	void Initalize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�I������
	void Finalize();

private: //�����o�ϐ�
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<ParticleManager> particles;

	NYMath::Vector3 cameraEye;
	WorldTransform transform;
private:
	GameScene(const GameScene&) = delete;
	const GameScene& operator=(const GameScene&) = delete;
};

