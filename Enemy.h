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

//自機クラスの前方宣言
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
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

	//行動フェーズ
	enum class Phase 
	{
		Approach,//接近する
		Leave,//離脱する
	};

	//敵の行動関数
	//接近
	void Aproach(Vector3 move, const float kEnemySpeed);
	//離脱
	void Leave(Vector3 move, const float kEnemySpeed);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	//接近フェーズの初期化
	void AproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

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

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//弾の間隔
	static const int32_t kFireInterval = 60;

	//間隔タイマー
	int32_t bulletTimer_ = kFireInterval;

	//自キャラ
	Player* player_ = nullptr;

	//ワールド座標を取得
	Vector3 GetWorldPosition();

};
