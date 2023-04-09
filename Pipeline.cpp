#include "Pipeline.h"

void Pipeline::CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, std::array<RootSigSetPip, 6>& pipeline)
{
	HRESULT result;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	// �G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz���W
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// ���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O���Ȃ�
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// �O�ʃJ�����O
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// �w�ʃJ�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// �[�x�N���b�s���O��L����
	// �u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0];
	// �u�����h�f�X�N
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA�S�Ẵ`�����l����`��
	// ���ʐݒ�
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	switch (blend)
	{
	case BlendMode::None: // �u�����h����
		blenddesc.BlendEnable = false;
		break;

	case BlendMode::Alpha: // �A���t�@
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendMode::Add: // ���Z
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::Sub: // ���Z
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::Mul: // ��Z
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendMode::Inv: // �F���]
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_COLOR;;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	}

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // �O�p�`
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	// ���[�g�p�����[�^�̐ݒ�
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���J��Ԃ�(�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// �c�J��Ԃ�(�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���s�J��Ԃ�(�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�b�v�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;												// �~�b�v�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_�k�̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline[static_cast<size_t>(blend)].rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = pipeline[static_cast<size_t>(blend)].rootSignature.Get();

	// �p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline[static_cast<size_t>(blend)].pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device,RootSigSetPip& pipeline)
{
	HRESULT result;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	// �G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz���W
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // �@���x�N�g��
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// ���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O���Ȃ�
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// �O�ʃJ�����O
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// �w�ʃJ�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// �[�x�N���b�s���O��L����
	// �u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0];
	// �u�����h�f�X�N
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA�S�Ẵ`�����l����`��
	// ���ʐݒ�
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	// ���܂�g��Ȃ����Ȃ̂ō��͔������̂�
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

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // �O�p�`
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�t�H�[�}�b�g
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// �萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�f�X�N���v�^�����W
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�X�N���v�^�����W��
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����o�b�t�@E

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���J��Ԃ�(�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// �c�J��Ԃ�(�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���s�J��Ԃ�(�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�b�v�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;												// �~�b�v�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_�k�̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = pipeline.rootSignature.Get();

	// �p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, BlendMode blend, ID3D12Device* device, RootSigSetPip& pipeline)
{
	HRESULT result;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	// �G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ //xyz���W
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ //uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ //�F
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// ���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O���Ȃ�
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// �O�ʃJ�����O
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// �w�ʃJ�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// �[�x�N���b�s���O��L����
	// �u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0];
	// �u�����h�f�X�N
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA�S�Ẵ`�����l����`��
	// ���ʐݒ�
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	// ���܂�g��Ȃ����Ȃ̂ō��͔������̂�
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

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT; // �O�p�`
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�t�H�[�}�b�g
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// �萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�f�X�N���v�^�����W
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�X�N���v�^�����W��
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����o�b�t�@E

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���J��Ԃ�(�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// �c�J��Ԃ�(�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���s�J��Ԃ�(�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�b�v�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;												// �~�b�v�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_�k�̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = pipeline.rootSignature.Get();

	// �p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}
