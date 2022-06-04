#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
		~PlayerBullet();
	/// <summary>

	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

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

	

};