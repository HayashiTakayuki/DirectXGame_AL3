#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Matrix.h"

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:
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

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//天球の行列計算
	Matrix* matrix_ = nullptr;
};