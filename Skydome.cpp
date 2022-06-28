#include"Skydome.h"

void Skydome::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	matrix_ = new Matrix();

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.scale_ = { 50,50,50 };

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	//���[���h�s��̌v�Z
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//�s��̍X�V
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjection_)
{
	//���L�����̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
