#pragma once
#include "NekoYanagiModel.h"

class FBXLoader
{
private:
	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	const aiScene* mScene;

};

