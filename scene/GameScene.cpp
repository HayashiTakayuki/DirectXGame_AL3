﻿#include "GameScene.h"
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

GameScene::~GameScene() 
{
	delete model_; 
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1600,900);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = { 5,5,5 };
	//スケーリング行列を宣言
	Matrix4 matScale;

	//単位行列を設定
	Matrix4 tani = {};
	tani.m[0][0] = 1;
	tani.m[1][1] = 1;
	tani.m[2][2] = 1;
	tani.m[3][3] = 1;

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	//X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = { Rad(45.0f),Rad(45.0f),0};

	Matrix4 matRot;
	Matrix4 matRotX,matRotY,matRotZ;

	//Z軸回転行列を宣言
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//X軸回転行列を宣言
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	matRotX.m[3][3] = 1;
	
	//Y軸回転行列を宣言
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	matRotY.m[3][3] = 1;
	
	worldTransform_.translation_ = {10,10,10};
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//平行移動行列を設定
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	//単位行列を代入する
	worldTransform_.matWorld_ = tani;

	//掛け算して代入
	//各軸の回転行列を合成
	matRot = matRotZ *= matRotX *= matRotY;
	
	//行列の合成
	worldTransform_.matWorld_ *= matScale *= matRot *= matTrans;

	//行列に転送
	worldTransform_.TransferMatrix();
}

void GameScene::Update() 
{
	debugCamera_->Update();
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

	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	
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
