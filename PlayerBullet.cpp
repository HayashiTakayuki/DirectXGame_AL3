#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
	delete matrix_;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NuLL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("mario.jpg");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�������������W���Z�b�g
	worldTransform_.translation_ = position;

}

void PlayerBullet::Upadate()
{
	//���[���h�s��̌v�Z
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//�s��̍X�V
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
