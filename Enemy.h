#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"
#include"EnemyBullet.h"
#include<memory>
#include<list>
#include"BaseEnemyState.h"
#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"

//自機クラスの前方宣言
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	//行動パターンを変える関数
	void ChageState(BaseEnemyState* newState);

	//座標を得るゲッター
	WorldTransform worldTransform() const { return worldTransform_; }

	//引数で指定した移動量だけ座標を変更する関数
	void EnemyMove(Vector3 move);

	//敵の行動パターン
	BaseEnemyState* state_;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	//接近フェーズの初期化
	void AproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//敵キャラから自キャラまでのベクトル
	Vector3 enemyToPlayer;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾のリストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	DebugText* debugText_ = nullptr;

	//プレイヤーの行列計算
	Matrix* matrix_ = nullptr;

	Input* input_ = nullptr;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//弾の間隔
	static const int32_t kFireInterval = 60;

	//間隔タイマー
	int32_t bulletTimer_ = kFireInterval;

	//自キャラ
	Player* player_ = nullptr;
	
	//敵キャラ
	Enemy* enemy_ = nullptr;
};
