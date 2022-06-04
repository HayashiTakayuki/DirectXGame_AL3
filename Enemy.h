#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"


/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy();

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

	//�s���t�F�[�Y
	enum class Phase 
	{
		Approach,//�ڋ߂���
		Leave,//���E����
	};

	//�G�̍s���֐�
	//�ڋ�
	void Aproach(Vector3 move, const float kEnemySpeed);
	//���E
	void Leave(Vector3 move, const float kEnemySpeed);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�v���C���[�̍s��v�Z
	Matrix* matrix_ = nullptr;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

};