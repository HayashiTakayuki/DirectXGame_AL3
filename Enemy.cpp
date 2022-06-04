#include "Enemy.h"

Enemy::~Enemy()
{
	delete matrix_;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//自キャラの生成
	matrix_ = new Matrix();

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f,3.0f,15.0f};

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Enemy::Update()
{
	//敵の移動ベクトル
	Vector3 move = { 0,0,0 };

	//敵の移動の速さ
	const float kEnemySpeed = 0.1f;

	switch (phase_)
	{
		//接近フェーズ
	case Phase::Approach:
	default:
		//移動関数
		Aproach(move, kEnemySpeed);
		break;
		//離脱フェーズ
	case Phase::Leave:
		//移動関数
		Leave(move, kEnemySpeed);

		break;
	}

	//注視点移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//ワールド行列の計算
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//移動限界座標
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

		//デバッグ用表示
	debugText_->SetPos(70, 170);
	debugText_->Printf(
		"Enemy:(%f,%f,%f)", worldTransform_.translation_.x += move.x,
		worldTransform_.translation_.y += move.y,
		worldTransform_.translation_.z += move.z);

	//行列の更新
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection viewProjection_)
{
	//自キャラの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Enemy::Aproach(Vector3 move, const float kEnemySpeed)
{
	debugText_->SetPos(70, 190);
	debugText_->Printf("Phase: Approach");
	//移動（ベクトル加算）
	move.z = -kEnemySpeed;
	worldTransform_.translation_ += move;
	//既定の位置についたら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave(Vector3 move, const float kEnemySpeed)
{
	debugText_->SetPos(70, 190);
	debugText_->Printf("Phase: Leave");

	//移動（ベクトル加算）
	move.x = -kEnemySpeed;
	move.y = kEnemySpeed;
	move.z = -kEnemySpeed;
	worldTransform_.translation_ += move;

}
