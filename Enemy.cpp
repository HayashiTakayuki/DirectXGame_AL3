#include "Enemy.h"
#include "Player.h"

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
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {12.0f,3.0f,15.0f};

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Enemy::Update()
{
	//�L�����N�^�[�̍U������
	AproachInitialize();

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
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

	//�e�̍X�V����
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Upadate();
	}

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

	debugText_->SetPos(70, 220);
	debugText_->Printf("bulletTimer%d",bulletTimer_);

	//�e�̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
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
	//move.x = -kEnemySpeed;
	//move.y = kEnemySpeed;
	//move.z = -kEnemySpeed;
	//worldTransform_.translation_ += move;

}

void Enemy::Fire()
{
	assert(player_);
	//�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	Vector3 enemyToPlayer;
	//�G�L����->���L�����̍����x�N�g�������߂�
	enemyToPlayer = player_->GetWorldPosition() - GetWorldPosition();
	//���K��
	enemyToPlayer.normalize();
	float bulletSpeed = 0.4f;

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = enemyToPlayer.normalize() * bulletSpeed;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::AproachInitialize()
{
	//���˃^�C�}�[�J�E���g�_�E��
	bulletTimer_--;
	//�w�莞�ԂɒB������
	if (bulletTimer_ <= 0)
	{
		//�e����
		Fire();
		//���˃^�C�}�[��������
		bulletTimer_ = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
