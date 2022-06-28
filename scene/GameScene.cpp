#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//プレイヤーの設定
	playerTextureHandle_ = TextureManager::Load("player.jpg");
	playerModel_ = Model::Create();

	//敵の設定
	enemyTextureHandle_ = TextureManager::Load("enemy.png");
	enemyModel_ = Model::Create();

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1600,900);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(playerModel_,playerTextureHandle_);

	enemy_= new Enemy();
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//自キャラの初期化
	enemy_->Initialize(enemyModel_, enemyTextureHandle_);

	//3Dモデルの生成
	modelSkydomeTextureHandle_ = TextureManager::Load("genso.jpg");
	modelSkydome_ = Model::CreateFromOBJ("tenkyu", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, modelSkydomeTextureHandle_);

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//	ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() 
{
	debugCamera_->Update();
	
	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	enemy_->Update();

	CheckAllCollision();

	skydome_->Update();
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
	//敵の描画
	enemy_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

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
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		//弾と弾の交差判定
		float x = posB.x - posA.x;
		float y = posB.y - posA.y;
		float z = posB.z - posA.z;

		//プレイヤーと敵弾の半径の合計幅
		float Radius = 3;

		if (pow(x,2) + pow(y,2) + pow(z,2)<=(Radius))
		{
			//自キャラの衝突時コールバック関数を呼び出す
			player_->OnCollision();

			//敵弾の衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posB = enemy_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		//自弾の座標
		posA = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		//弾と弾の交差判定
		float x = posB.x - posA.x;
		float y = posB.y - posA.y;
		float z = posB.z - posA.z;

		//プレイヤーと敵弾の半径の合計幅
		float Radius = 4;

		if (pow(x, 2) + pow(y, 2) + pow(z, 2) <= (Radius))
		{
			//敵キャラの衝突時コールバック関数を呼び出す
			enemy_->OnCollision();

			//自弾の衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			//自弾の座標
			posA = playerBullet->GetWorldPosition();
			posB = enemyBullet->GetWorldPosition();
			//座標AとBの距離を求める
			//弾と弾の交差判定
			float x = posB.x - posA.x;
			float y = posB.y - posA.y;
			float z = posB.z - posA.z;

			//プレイヤーと敵弾の半径の合計幅
			float Radius = 3;

			if (pow(x, 2) + pow(y, 2) + pow(z, 2) <= (Radius))
			{
				//自弾の衝突時コールバック関数を呼び出す
				playerBullet->OnCollision();

				//敵弾の衝突時コールバック関数を呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
