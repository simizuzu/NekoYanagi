#pragma once
#include "NekoYanagiModel.h"

class FBXLoader
{
private:
	//�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	const aiScene* mScene;

};

