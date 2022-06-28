#include"Skydome.h"

void Skydome::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	matrix_ = new Matrix();

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.scale_ = { 50,50,50 };

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	//ワールド行列の計算
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//行列の更新
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjection_)
{
	//自キャラの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
