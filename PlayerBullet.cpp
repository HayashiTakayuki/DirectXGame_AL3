#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
	delete matrix_;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NuLLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//因数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;

}

void PlayerBullet::Upadate()
{
	//ワールド行列の計算
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//行列の更新
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
