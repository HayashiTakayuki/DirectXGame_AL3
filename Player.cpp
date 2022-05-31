#include "Player.h"

//平行移動行列を設定
Matrix4 matTrans(WorldTransform worldTransform_)
{
	Matrix4 matTrans;
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	return matTrans;
}

//単位行列を設定
Matrix4 tani()
{
	Matrix4 tani = {};
	tani.m[0][0] = 1;
	tani.m[1][1] = 1;
	tani.m[2][2] = 1;
	tani.m[3][3] = 1;
	return tani;
}

//行列の設定
Matrix4 matrix(WorldTransform worldTransform_)
{
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//単位行列を設定
	tani();

	//平行移動行列を設定
	matTrans(worldTransform_);

	//単位行列を代入する
	worldTransform_.matWorld_ = tani();

	//掛け算して代入

	//行列の合成
	worldTransform_.matWorld_ *= matTrans(worldTransform_);

	return worldTransform_.matWorld_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0, 0 };
	}

	//注視点移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//行列の更新
	worldTransform_.TransferMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x += move.x,
		worldTransform_.translation_.y += move.y,
		worldTransform_.translation_.z += move.z);

}

void Player::Draw(ViewProjection viewProjection_)
{
	//自キャラの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
