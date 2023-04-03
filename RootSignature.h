#pragma once
#include "AvoidWarning.h"

class RootSignature
{
public:
	static void CreateSpriteRootSig(Microsoft::WRL::ComPtr<ID3DBlob>& rootSigBlob, Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob);
};

