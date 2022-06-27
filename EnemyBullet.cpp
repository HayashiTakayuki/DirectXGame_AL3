#include "EnemyBullet.h"
#include "Player.h"

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
	debugText_ = DebugText::GetInstance();

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//�e�̌`��
	worldTransform_.scale_ = { 1,1,1 };

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�������������W���Z�b�g
	worldTransform_.translation_ = position;
}

void EnemyBullet::Upadate()
{
	assert(player_);
	//�G�e->���L�����̍����x�N�g�������߂�
	enemyBulletToPlayer = player_->GetWorldPosition() - GetWorldEnemyBulletPosition();
	//���K��
	enemyBulletToPlayer.normalize();
	velocity_.normalize();

	//���ʐ��`�⊮�ɂ��,���̑��x�Ǝ��L�����ւ̃x�N�g������ς��A�V���ȑ��x�Ƃ���
	velocity_ = Slerp(velocity_, enemyBulletToPlayer, 1) * bulletSpeed;

	//Y������p�x(��y)
	worldTransform_.rotation_.y = std::atan2(enemyBulletToPlayer.x, enemyBulletToPlayer.z);
	//���������̒���
	Vector3 a = { enemyBulletToPlayer.x,0, enemyBulletToPlayer.z };

	//X������p�x(��x)
	worldTransform_.rotation_.x = std::atan2(-enemyBulletToPlayer.y, a.length());

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

Vector3 EnemyBullet::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return v1 + t * v2;
}

Vector3 EnemyBullet::Slerp(Vector3& v1,Vector3& v2, float t)
{
	Vector3 v;
	v1.normalize();
	v2.normalize();

	//�Q�x�N�g���Ԃ̊p�x
	float angle = acos(v1.dot(v2));

	//sin��
	float sinTh = sin(angle);

	//�⊮�W��
	float Ps = sin(angle * (1 - t));
	float Pe = sin(angle * t);

	v = (Ps * v1 + Pe * v2) / sinTh;

	//�ꉞ���K�����ċ��ʕ�Ԃ�
	v.normalize();

	return v;
}

Vector3 EnemyBullet::GetWorldEnemyBulletPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
