#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	CreateConstBuffer();
}

void WorldTransform::Update(Camera* camera, bool billboradFlag)
{
	HRESULT result;
	NYMath::Matrix4 matScale, matRot, matTrans;

	//スケール、回転平行移動行列の計算
	matScale = NYMath::MakeScaling(scale_);
	matRot = NYMath::MakeRotation(rotation_);
	matTrans = NYMath::MakeTranslation(translation_);

	//ワールド行列の合成
	matWorld = NYMath::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= matScale;
	//ワールド行列に回転を反映
	matWorld *= matRot;
	//ワールド行列に平行移動を反映
	matWorld *= matTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	if (!billboradFlag)
	{
		const NYMath::Matrix4 matView = camera->GetMatView();
		const NYMath::Matrix4 matProjection = camera->GetMatProjection();

		//定数バッファへデータ転送
		ConstBufferDataWorldTransform* constMap = nullptr;
		result = constBuffer_->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result));
		constMap->mat = matWorld * matView * matProjection;
		constBuffer_->Unmap(0, nullptr);
	}

}

D3D12_GPU_VIRTUAL_ADDRESS WorldTransform::GetGpuAddress()
{
	return constBuffer_->GetGPUVirtualAddress();
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer_));
	assert(SUCCEEDED(result));
	constBuffer_->SetName(L"WorldTransform");
}
