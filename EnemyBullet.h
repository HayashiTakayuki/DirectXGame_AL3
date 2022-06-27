#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"

//���@�N���X�̑O���錾
class Player;

/// <summary>
/// �G�̒e
/// </summary>
class EnemyBullet 
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EnemyBullet();
	/// <summary>

	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Upadate();

	/// <summary>
	/// �e�̕`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

	//���`�⊮
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	//���`���
	Vector3 Slerp(Vector3& v1,Vector3& v2, float t);

	//���[���h���W���擾
	Vector3 GetWorldEnemyBulletPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	Vector3 GetWorldPosition();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//�s��̌v�Z
	Matrix* matrix_ = nullptr;

	//���x
	Vector3 velocity_;

	//�e�̎���
	static const int32_t kLileTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLileTime;
	//�f�X�t���O
	bool isDead_ = false;

	//���L����
	Player* player_ = nullptr;

	//�G�L�������玩�L�����܂ł̃x�N�g��
	Vector3 enemyBulletToPlayer;

	float bulletSpeed = 0.3f;

	float time = 0.0f;
};