#include"EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateLeave::Update(Enemy* pEnemy)
{
	//�G�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�G�̈ړ��̑���
	const float kEnemySpeed = 0.1f;

	//�ړ��i�x�N�g�����Z�j
	//move.x = -kEnemySpeed;
	//move.y = kEnemySpeed;
	//move.z = -kEnemySpeed;

	pEnemy->EnemyMove(move);

}