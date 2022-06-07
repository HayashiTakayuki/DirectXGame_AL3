#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"
#include"EnemyBullet.h"
#include<memory>
#include<list>

//���@�N���X�̑O���錾
class Player;

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

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	//�ڋ߃t�F�[�Y�̏�����
	void AproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	DebugText* debugText_ = nullptr;

	//�v���C���[�̍s��v�Z
	Matrix* matrix_ = nullptr;

	Input* input_ = nullptr;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//�e�̊Ԋu
	static const int32_t kFireInterval = 60;

	//�Ԋu�^�C�}�[
	int32_t bulletTimer_ = kFireInterval;

	//���L����
	Player* player_ = nullptr;

	//���[���h���W���擾
	Vector3 GetWorldPosition();

};
