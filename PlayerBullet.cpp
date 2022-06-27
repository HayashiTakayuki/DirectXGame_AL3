#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
	delete matrix_;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NuLLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("bullet.png");

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//因数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Upadate()
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

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision()
{

	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
