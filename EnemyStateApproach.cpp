#include"EnemyStateApproach.h"
#include"Enemy.h"

void EnemyStateApproach::Update(Enemy* pEnemy)
{
	//敵の移動ベクトル
	Vector3 move = { 0,0,0 };
	//敵の移動の速さ
	const float kEnemySpeed = 0.1f;

	//移動（ベクトル加算）
	move.z = -kEnemySpeed;

	pEnemy->EnemyMove(move);

	//既定の位置についたら離脱
	if (pEnemy->worldTransform().translation_.z < 0.0f)
	{
		pEnemy->ChageState(new EnemyStateLeave);
	}
}