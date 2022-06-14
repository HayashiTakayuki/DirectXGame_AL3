#include"EnemyStateApproach.h"
#include"Enemy.h"

void EnemyStateApproach::Update(Enemy* pEnemy)
{
	//�G�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�G�̈ړ��̑���
	const float kEnemySpeed = 0.1f;

	//�ړ��i�x�N�g�����Z�j
	move.z = -kEnemySpeed;

	pEnemy->EnemyMove(move);

	//����̈ʒu�ɂ����痣�E
	if (pEnemy->worldTransform().translation_.z < 0.0f)
	{
		pEnemy->ChageState(new EnemyStateLeave);
	}
}