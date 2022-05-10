﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

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
