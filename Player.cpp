#include "Player.h"

Player::~Player()
{
	delete matrix_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//���L�����̐���
	matrix_ = new Matrix();

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
		move.x = -kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x = kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y = kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y = -kCharacterSpeed;
	}

	//��]����
	{
		//��]�̑���[���W�A��/flame]
		const float kChectSpeed = 0.05f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_U))
		{
			worldTransform_.rotation_.y -= kChectSpeed;
		}
		if (input_->PushKey(DIK_I))
		{
			worldTransform_.rotation_.y += kChectSpeed;
		}
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

	//�s��̍X�V
	worldTransform_.TransferMatrix();

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x += move.x,
		worldTransform_.translation_.y += move.y,
		worldTransform_.translation_.z += move.z);

	//�L�����N�^�[�̍U������
	Attack();
	//�e�̍X�V����
	if (bullet_) 
	{
		bullet_->Upadate();
	}
}

void Player::Draw(ViewProjection viewProjection_)
{
	//���L�����̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//�e�̕`��
	if (bullet_)
	{
		bullet_->Draw(viewProjection_);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		PlayerBullet* newBullet = new PlayerBullet();
		
		newBullet->Initialize(model_, worldTransform_.translation_);

		//�e��o�^����
		bullet_ = newBullet;
	}
}