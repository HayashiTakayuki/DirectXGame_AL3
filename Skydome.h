#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Matrix.h"

/// <summary>
/// �V��
/// </summary>
class Skydome
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�V���̍s��v�Z
	Matrix* matrix_ = nullptr;
};