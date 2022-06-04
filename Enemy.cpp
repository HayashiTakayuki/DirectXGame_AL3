#include "Enemy.h"

Enemy::~Enemy()
{
	delete matrix_;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//���L�����̐���
	matrix_ = new Matrix();

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f,3.0f,15.0f};

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Enemy::Update()
{
	//�G�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	//�G�̈ړ��̑���
	const float kEnemySpeed = 0.1f;

	switch (phase_)
	{
		//�ڋ߃t�F�[�Y
	case Phase::Approach:
	default:
		//�ړ��֐�
		Aproach(move, kEnemySpeed);
		break;
		//���E�t�F�[�Y
	case Phase::Leave:
		//�ړ��֐�
		Leave(move, kEnemySpeed);

		break;
	}

	//�����_�ړ��i�x�N�g���̉��Z�j
	worldTransform_.translation_ += move;

	//���[���h�s��̌v�Z
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//�ړ����E���W
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

		//�f�o�b�O�p�\��
	debugText_->SetPos(70, 170);
	debugText_->Printf(
		"Enemy:(%f,%f,%f)", worldTransform_.translation_.x += move.x,
		worldTransform_.translation_.y += move.y,
		worldTransform_.translation_.z += move.z);

	//�s��̍X�V
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection viewProjection_)
{
	//���L�����̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Enemy::Aproach(Vector3 move, const float kEnemySpeed)
{
	debugText_->SetPos(70, 190);
	debugText_->Printf("Phase: Approach");
	//�ړ��i�x�N�g�����Z�j
	move.z = -kEnemySpeed;
	worldTransform_.translation_ += move;
	//����̈ʒu�ɂ����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave(Vector3 move, const float kEnemySpeed)
{
	debugText_->SetPos(70, 190);
	debugText_->Printf("Phase: Leave");

	//�ړ��i�x�N�g�����Z�j
	move.x = -kEnemySpeed;
	move.y = kEnemySpeed;
	move.z = -kEnemySpeed;
	worldTransform_.translation_ += move;

}
