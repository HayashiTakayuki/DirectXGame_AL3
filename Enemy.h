#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"


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

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//プレイヤーの行列計算
	Matrix* matrix_ = nullptr;

	//フェーズ
	Phase phase_ = Phase::Approach;

};