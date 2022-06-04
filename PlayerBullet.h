#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
		~PlayerBullet();
	/// <summary>

	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Upadate();

	/// <summary>
	/// 弾の描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//行列の計算
	Matrix* matrix_ = nullptr;

	//速度
	Vector3 velocity_;

	//弾の寿命
	static const int32_t kLileTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLileTime;
	//デスフラグ
	bool isDead_ = false;

	

};