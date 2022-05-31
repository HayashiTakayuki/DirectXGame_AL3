#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>

/// <summary>
/// 自キャラ
/// </summary>
class Player 
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;	
	DebugText* debugText_ = nullptr;
	
	//プレイヤーの行列計算
	Matrix* matrix_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};