#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet()
{
	delete matrix_;
}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NuLLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//弾の形状
	worldTransform_.scale_ = { 0.5f,0.5f,3.0f };

	//Y軸周り角度(θy)
	//worldTransform_.rotation_.y = std::atan2(,);
	//横軸方向の長さ

	//X軸周り角度(θx)
	//worldTransform_.rotation_.y = std::atan2(,);

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//因数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;
}

void EnemyBullet::Upadate()
{
	//ワールド行列の計算
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//座標を移動させる（１フレームの移動量を足しこむ）
	worldTransform_.translation_ += velocity_;

	//行列の更新
	worldTransform_.TransferMatrix();

	//時間経過で消える
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
