#pragma once
#include "Camera.h"

class GameScene
{
public: //メンバ変数
	//コンストラクタ・デストラクタ
	GameScene();
	~GameScene();
	//初期化
	void Initalize();

	//更新
	void Update();

	//描画
	void Draw();

	//終了処理
	void Finalize();

private: //メンバ変数
	std::unique_ptr<Camera> camera_;

private:
	GameScene(const GameScene&) = delete;
	const GameScene& operator=(const GameScene&) = delete;
};

