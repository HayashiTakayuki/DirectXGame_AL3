#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

//度数法を弧度法にする関数
float Rad(float dosu) 
{
	float PAI = 3.14159265358979323f;

	return dosu / 180 * PAI;
}

//スケーリング行列を宣言
Matrix4 matScale(WorldTransform worldTransform_)
{
	Matrix4 matScale;
	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;
	return matScale;
}

//Z軸回転行列を宣言
Matrix4 matRotZ(WorldTransform worldTransform_)
{
	Matrix4 matRotZ;
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
	
	return matRotZ;
}

//X軸回転行列を宣言
Matrix4 matRotX(WorldTransform worldTransform_)
{
	Matrix4 matRotX;
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	matRotX.m[3][3] = 1;

	return matRotX;
}

//Y軸回転行列を宣言
Matrix4 matRotY(WorldTransform worldTransform_)
{
	Matrix4 matRotY;
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	matRotY.m[3][3] = 1;

	return matRotY;
}

//平行移動行列を設定
//Matrix4 matTrans(WorldTransform worldTransform_)
//{
//	Matrix4 matTrans;
//	matTrans.m[0][0] = 1;
//	matTrans.m[1][1] = 1;
//	matTrans.m[2][2] = 1;
//	matTrans.m[3][3] = 1;
//	matTrans.m[3][0] = worldTransform_.translation_.x;
//	matTrans.m[3][1] = worldTransform_.translation_.y;
//	matTrans.m[3][2] = worldTransform_.translation_.z;
//	return matTrans;
//}
//
////単位行列を設定
//Matrix4 tani()
//{
//	Matrix4 tani = {};
//	tani.m[0][0] = 1;
//	tani.m[1][1] = 1;
//	tani.m[2][2] = 1;
//	tani.m[3][3] = 1;
//	return tani;
//}
//
////行列の設定
//Matrix4 matrix(WorldTransform worldTransform_)
//{
//	//ワールドトランスフォームの初期化
//	worldTransform_.Initialize();
//
//	//単位行列を設定
//	tani();
//
//	//スケーリング倍率を行列に設定する
//	matScale(worldTransform_);
//
//	//Z軸回転行列を宣言
//	matRotZ(worldTransform_);
//
//	//X軸回転行列を宣言
//	matRotX(worldTransform_);
//
//	//Y軸回転行列を宣言
//	matRotY(worldTransform_);
//
//	//平行移動行列を設定
//	matTrans(worldTransform_);
//
//	//単位行列を代入する
//	worldTransform_.matWorld_ = tani();
//
//	//掛け算して代入
//	//各軸の回転行列を合成
//	Matrix4 matRot;
//
//	matRot = matRotZ(worldTransform_) *= matRotX(worldTransform_) *= matRotY(worldTransform_);
//
//	//行列の合成
//	worldTransform_.matWorld_ *= matScale(worldTransform_) *= matRot *= matTrans(worldTransform_);
//
//
//	return worldTransform_.matWorld_;
//}

GameScene::~GameScene() 
{
	delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1600,900);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);


	//	ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() 
{
	debugCamera_->Update();
	
	//自キャラの更新
	player_->Update();

}

void GameScene::Draw() {

	//デバッグのグリッド線
	for (int i = -10; i <= 10; i++)
	{
		//X軸
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(-10.0f, 0.0f, 0.0f + i), Vector3(10.0f, 0.0f, 0.0f + i), Vector4(0, 0, 255, 255));
		//Y軸
		//PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(-10.0f, 0.0f+ i, 0.0f ), Vector3(10.0f, 0.0f + i, 0.0f), Vector4(0, 255, 0, 255));
		//Z軸
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0.0f + i, 0.0f, -10.0f), Vector3(0.0f + i, 0.0f, 10.0f), Vector4(255, 0, 0, 255));

	}
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//自キャラの描画
	player_->Draw(viewProjection_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
