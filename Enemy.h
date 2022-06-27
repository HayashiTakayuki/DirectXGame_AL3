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
#include"BaseEnemyState.h"
#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"

//���@�N���X�̑O���錾
class Player;

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy();

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

	//�s���p�^�[����ς���֐�
	void ChageState(BaseEnemyState* newState);

	//���W�𓾂�Q�b�^�[
	WorldTransform worldTransform() const { return worldTransform_; }

	//�����Ŏw�肵���ړ��ʂ������W��ύX����֐�
	void EnemyMove(Vector3 move);

	//�G�̍s���p�^�[��
	BaseEnemyState* state_;

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	//�ڋ߃t�F�[�Y�̏�����
	void AproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�G�L�������玩�L�����܂ł̃x�N�g��
	Vector3 enemyToPlayer;

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e�̃��X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

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

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//�e�̊Ԋu
	static const int32_t kFireInterval = 60;

	//�Ԋu�^�C�}�[
	int32_t bulletTimer_ = kFireInterval;

	//���L����
	Player* player_ = nullptr;
	
	//�G�L����
	Enemy* enemy_ = nullptr;
};
