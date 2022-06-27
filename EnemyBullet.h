#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "assert.h"
#include "Input.h"
#include "DebugText.h"
#include "Matrix.h"

//自機クラスの前方宣言
class Player;

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet 
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet();
	/// <summary>

	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

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

	void SetPlayer(Player* player) { player_ = player; }

	//線形補完
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	//線形補間
	Vector3 Slerp(Vector3& v1,Vector3& v2, float t);

	//ワールド座標を取得
	Vector3 GetWorldEnemyBulletPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();
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

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラから自キャラまでのベクトル
	Vector3 enemyBulletToPlayer;

	float bulletSpeed = 0.3f;

	float time = 0.0f;
};