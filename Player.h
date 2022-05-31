#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>

/// <summary>
/// ���L����
/// </summary>
class Player 
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

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

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;	
	DebugText* debugText_ = nullptr;
	
	//�v���C���[�̍s��v�Z
	Matrix* matrix_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};