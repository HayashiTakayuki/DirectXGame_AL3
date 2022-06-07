#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet()
{
	delete matrix_;
}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NuLL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("mario.jpg");

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//�e�̌`��
	worldTransform_.scale_ = { 0.5f,0.5f,3.0f };

	//Y������p�x(��y)
	//worldTransform_.rotation_.y = std::atan2(,);
	//���������̒���

	//X������p�x(��x)
	//worldTransform_.rotation_.y = std::atan2(,);

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�������������W���Z�b�g
	worldTransform_.translation_ = position;
}

void EnemyBullet::Upadate()
{
	//���[���h�s��̌v�Z
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//���W���ړ�������i�P�t���[���̈ړ��ʂ𑫂����ށj
	worldTransform_.translation_ += velocity_;

	//�s��̍X�V
	worldTransform_.TransferMatrix();

	//���Ԍo�߂ŏ�����
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
