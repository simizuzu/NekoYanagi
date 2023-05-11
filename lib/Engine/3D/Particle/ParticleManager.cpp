#include "ParticleManager.h"
#include "NekoYanagiUtility.h"

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
Microsoft::WRL::ComPtr<ID3D12Device> ParticleManager::device_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> ParticleManager::cmdList_;
RootSigSetPip ParticleManager::pip;

UINT ParticleManager::descriptorHandleIncrementSize = 0;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap;
Microsoft::WRL::ComPtr<ID3D12Resource> ParticleManager::vertBuff;
Microsoft::WRL::ComPtr<ID3D12Resource> ParticleManager::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE ParticleManager::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE ParticleManager::gpuDescHandleSRV;

D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
VertexPos ParticleManager::vertices[vertexCount];

NYMath::Matrix4 ParticleManager::matView;
NYMath::Matrix4 ParticleManager::matProjection{};

ParticleManager::~ParticleManager()
{
	device_.Reset();
	descHeap.Reset();
	cmdList_.Reset();
	vertBuff.Reset();
	texbuff.Reset();

	pip.pipelineState.Reset();
	pip.rootSignature.Reset();
}

void ParticleManager::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	assert(device);
	ParticleManager::device_ = device;
	ParticleManager::cmdList_ = cmdList;
	InitializeDescriptorHeap();
	CreateCraphicsPipeline();
	CreateModel();
}

void ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	if (FAILED(result)) {
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ParticleManager::CreateCraphicsPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;

	Shader::CreateParticleShade(vsBlob, psBlob, gsBlob);
	Pipeline::CreatePaticlePipeline(vsBlob.Get(), psBlob.Get(), gsBlob.Get(), device_.Get(), pip);
}

void ParticleManager::Update()
{
	HRESULT result;

	// �������s�����p�[�e�B�N����S�폜
	particles.remove_if([](Particle& x) {return x.frame >= x.num_frame; });

	// �S�p�[�e�B�N���X�V
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		// �o�߃t���[�������J�E���g
		it->frame++;
		// ���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		// ���x�ɂ��ړ�
		it->position = it->position + it->velocity;

		// �i�s�x��0�`1�͈̔͂Ɋ��Z
		float f = (float)it->frame / (float)it->num_frame;
		// �X�P�[���̐��`���
		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;

		//�Ԃ̐��`���
		it->color.x = (it->e_color.x - it->s_color.x) * f;
		it->color.x += it->s_color.x;
		//�̐��`���
		it->color.y = (it->e_color.y - it->s_color.y) * f;
		it->color.y += it->s_color.y;
		//�΂̐��`���
		it->color.z = (it->e_color.z - it->s_color.z) * f;
		it->color.z += it->s_color.z;
	}

	// ���_�o�b�t�@�փf�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	uint16_t count = 0;
	if (SUCCEEDED(result)) {
		// �p�[�e�B�N���̏���������f
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
			it++) {

			if (count == vertexCount)
			{
				break;
			}
			// ���W
			vertMap->pos = it->position;
			// �X�P�[��
			vertMap->scale = it->scale;
			// �F
			vertMap->color = it->color;
			// ���̒��_��
			vertMap++;

			count++;
		}
		vertBuff->Unmap(0, nullptr);
	}
}

void ParticleManager::Add(int life, NYMath::Vector3 position, NYMath::Vector3 velocity, NYMath::Vector3 accel, float start_scale, float end_scale, NYMath::Vector4 s_color, NYMath::Vector4 e_color)
{
	//���X�g�ɗv�f��ǉ�
	particles.emplace_front();
	//�ǉ������v�f�̎Q��
	Particle& p = particles.front();
	//�l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;

	p.s_color = s_color;
	p.e_color = e_color;
}

void ParticleManager::RandParticle()
{
	for (int i = 0; i < 30; i++)
	{
		//X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float rnd_pos = 10.0f;
		NYMath::Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//X,Y,Z�S��[-0.05f,+0.05f]�Ń����_���ɕ��z
		const float rnd_vel = 0.3f;
		NYMath::Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		// �d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		NYMath::Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		NYMath::Vector4 col{};
		const float rnd_col = 1.0f;
		col.x = (float)rand() / RAND_MAX * rnd_col;
		col.y = (float)rand() / RAND_MAX * rnd_col;
		col.z = (float)rand() / RAND_MAX * rnd_col;

		// �ǉ�
		Add(60, pos, vel, acc, 1.0f, 0.0f, { 1,0,1,1 }, { 1,0,1,1 });
	}
}

void ParticleManager::Draw(WorldTransform* transform)
{
	// nullptr�`�F�b�N
	assert(device_);
	assert(ParticleManager::cmdList_);

	// ���_�o�b�t�@�̐ݒ�
	cmdList_->IASetVertexBuffers(0, 1, &vbView);

		// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList_->SetPipelineState(pip.pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList_->SetGraphicsRootSignature(pip.rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList_->SetGraphicsRootConstantBufferView(0, transform->GetGpuAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList_->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// �`��R�}���h_countof(indices)
		// �`��R�}���h
	if (std::distance(particles.begin(), particles.end()) < vertexCount)
	{
		cmdList_->DrawInstanced(static_cast<UINT> (std::distance(particles.begin(), particles.end())), 1, 0, 0);
	}
	else
	{
		cmdList_->DrawInstanced(vertexCount, 1, 0, 0);
	}

	cmdList_->SetName(L"ParticleCmdList");
}

void ParticleManager::LoadTexture(const wchar_t* texturename)
{
	HRESULT result = S_FALSE;

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(texturename, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	DirectX::ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = DirectX::MakeSRGB(metadata.format);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);


	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr, IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));

	texbuff->SetName(L"ParticleTexbuff");

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0); // ���f�[�^���o
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	descHeap->SetName(L"ParticleDescHeap");

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(vertices));

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// ���_�o�b�t�@����
	result = device_->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	device_->SetName(L"ParticleDevice");

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}
