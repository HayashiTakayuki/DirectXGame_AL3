#include "Player.h"

//���s�ړ��s���ݒ�
Matrix4 matTrans(WorldTransform worldTransform_)
{
	Matrix4 matTrans;
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	return matTrans;
}

//�P�ʍs���ݒ�
Matrix4 tani()
{
	Matrix4 tani = {};
	tani.m[0][0] = 1;
	tani.m[1][1] = 1;
	tani.m[2][2] = 1;
	tani.m[3][3] = 1;
	return tani;
}

//�s��̐ݒ�
Matrix4 matrix(WorldTransform worldTransform_)
{
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�P�ʍs���ݒ�
	tani();

	//���s�ړ��s���ݒ�
	matTrans(worldTransform_);

	//�P�ʍs���������
	worldTransform_.matWorld_ = tani();

	//�|���Z���đ��

	//�s��̍���
	worldTransform_.matWorld_ *= matTrans(worldTransform_);

	return worldTransform_.matWorld_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0, 0 };
	}

	//�����_�ړ��i�x�N�g���̉��Z�j
	worldTransform_.translation_ += move;

	//�s��̍X�V
	worldTransform_.TransferMatrix();

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x += move.x,
		worldTransform_.translation_.y += move.y,
		worldTransform_.translation_.z += move.z);

}

void Player::Draw(ViewProjection viewProjection_)
{
	//���L�����̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
