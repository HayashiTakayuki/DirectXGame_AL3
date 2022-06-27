#include "EnemyBullet.h"
#include "Player.h"

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
	debugText_ = DebugText::GetInstance();

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//弾の形状
	worldTransform_.scale_ = { 1,1,1 };

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//因数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;
}

void EnemyBullet::Upadate()
{
	assert(player_);
	//敵弾->自キャラの差分ベクトルを求める
	enemyBulletToPlayer = player_->GetWorldPosition() - GetWorldEnemyBulletPosition();
	//正規化
	enemyBulletToPlayer.normalize();
	velocity_.normalize();

	//球面線形補完により,今の速度と自キャラへのベクトルを内積し、新たな速度とする
	velocity_ = Slerp(velocity_, enemyBulletToPlayer, 1) * bulletSpeed;

	//Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(enemyBulletToPlayer.x, enemyBulletToPlayer.z);
	//横軸方向の長さ
	Vector3 a = { enemyBulletToPlayer.x,0, enemyBulletToPlayer.z };

	//X軸周り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-enemyBulletToPlayer.y, a.length());

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

Vector3 EnemyBullet::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return v1 + t * v2;
}

Vector3 EnemyBullet::Slerp(Vector3& v1,Vector3& v2, float t)
{
	Vector3 v;
	v1.normalize();
	v2.normalize();

	//２ベクトル間の角度
	float angle = acos(v1.dot(v2));

	//sinθ
	float sinTh = sin(angle);

	//補完係数
	float Ps = sin(angle * (1 - t));
	float Pe = sin(angle * t);

	v = (Ps * v1 + Pe * v2) / sinTh;

	//一応正規化して球面補間に
	v.normalize();

	return v;
}

Vector3 EnemyBullet::GetWorldEnemyBulletPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
