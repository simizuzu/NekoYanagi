#include "Pipeline.h"

void Pipeline::CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, std::array<RootSigSetPip, 6>& pipeline)
{
	HRESULT result;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// 前面カリング
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// 深度クリッピングを有効に
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0];
	// ブレンドデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA全てのチャンネルを描画
	// 共通設定
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	switch (blend)
	{
	case BlendMode::None: // ブレンド無し
		blenddesc.BlendEnable = false;
		break;

	case BlendMode::Alpha: // アルファ
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendMode::Add: // 加算
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::Sub: // 減算
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::Mul: // 乗算
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendMode::Inv: // 色反転
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_COLOR;;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 三角形
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	// ルートパラメータの設定
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 横繰り返し(タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 縦繰り返し(タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 奥行繰り返し(タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// ミップマップ最大値
	sampleDesc.MinLOD = 0.0f;												// ミップマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダ殻のみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline[static_cast<size_t>(blend)].rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipeline[static_cast<size_t>(blend)].rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline[static_cast<size_t>(blend)].pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device,RootSigSetPip& pipeline)
{
	HRESULT result;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // 法線ベクトル
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// 前面カリング
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// 深度クリッピングを有効に
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0];
	// ブレンドデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA全てのチャンネルを描画
	// 共通設定
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	// あまり使わなそうなので今は半透明のみ
	if (blend == BlendMode::Alpha)
	{
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}
	else
	{
		blenddesc.BlendEnable = false;
	}

	if (blend == BlendMode::None)
	{
		blenddesc.BlendEnable = false;
	}
	else
	{
		blenddesc.BlendEnable = false;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 三角形
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//デスクリプタレンジ
	rootParams[2].Descriptor.RegisterSpace = 0;						//デスクリプタレンジ数
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファE

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 横繰り返し(タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 縦繰り返し(タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 奥行繰り返し(タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// ミップマップ最大値
	sampleDesc.MinLOD = 0.0f;												// ミップマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダ殻のみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipeline.rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, BlendMode blend, ID3D12Device* device, RootSigSetPip& pipeline)
{
	HRESULT result;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ //xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ //uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ //色
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// 前面カリング
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// 深度クリッピングを有効に
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0];
	// ブレンドデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA全てのチャンネルを描画
	// 共通設定
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	// あまり使わなそうなので今は半透明のみ
	if (blend == BlendMode::Alpha)
	{
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}
	else
	{
		blenddesc.BlendEnable = false;
	}

	if (blend == BlendMode::None)
	{
		blenddesc.BlendEnable = false;
	}
	else
	{
		blenddesc.BlendEnable = false;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT; // 三角形
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//デスクリプタレンジ
	rootParams[2].Descriptor.RegisterSpace = 0;						//デスクリプタレンジ数
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファE

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 横繰り返し(タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 縦繰り返し(タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 奥行繰り返し(タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// ミップマップ最大値
	sampleDesc.MinLOD = 0.0f;												// ミップマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダ殻のみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipeline.rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}
