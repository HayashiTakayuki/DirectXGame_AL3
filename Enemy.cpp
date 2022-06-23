#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
	//アプローチクラスのnew
	state_ = new EnemyStateApproach();
}

Enemy::~Enemy()
{
	delete matrix_;
	delete state_;
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
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {12.0f,3.0f,15.0f};

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Enemy::Update()
{
	//敵の行動パターンのアップデート
	state_->Update(this);

	//キャラクターの攻撃処理
	AproachInitialize();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	//敵の移動ベクトル
	Vector3 move = { 0,0,0 };

	//敵の移動の速さ
	const float kEnemySpeed = 0.1f;

	//注視点移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//ワールド行列の計算
	worldTransform_.matWorld_ = matrix_->matrix(worldTransform_);

	//弾の更新処理
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Upadate();
	}

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

	debugText_->SetPos(70, 220);
	debugText_->Printf("bulletTimer%d",bulletTimer_);

	//弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

//敵の行動パターンを変える
void Enemy::ChageState(BaseEnemyState* newState)
{
	delete state_;
	state_ = newState;
}

//引数で指定した移動量だけ座標を変更する関数
void Enemy::EnemyMove(Vector3 move)
{
	worldTransform_.translation_ += move;
}

void Enemy::Fire()
{
	assert(player_);
	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//敵キャラ->自キャラの差分ベクトルを求める
	enemyToPlayer = player_->GetWorldPosition() - GetWorldPosition();
	//正規化
	enemyToPlayer.normalize();
	float bulletSpeed = 0.4f;

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = enemyToPlayer.normalize() * bulletSpeed;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	newBullet->SetPlayer(player_);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::AproachInitialize()
{
	//発射タイマーカウントダウン
	bulletTimer_--;
	//指定時間に達したら
	if (bulletTimer_ <= 0)
	{
		//弾発射
		Fire();
		//発射タイマーを初期化
		bulletTimer_ = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
