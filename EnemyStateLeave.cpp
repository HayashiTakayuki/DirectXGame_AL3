#include"EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateLeave::Update(Enemy* pEnemy)
{
	//敵の移動ベクトル
	Vector3 move = { 0,0,0 };
	//敵の移動の速さ
	const float kEnemySpeed = 0.1f;

	//移動（ベクトル加算）
	//move.x = -kEnemySpeed;
	//move.y = kEnemySpeed;
	//move.z = -kEnemySpeed;

	pEnemy->EnemyMove(move);

}