#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include<random>

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
	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1600,900);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, PAI*2.0f);
	//乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//範囲forですべてのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();
		//X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = { 1.0f, 1.0f, 1.0f };
		//X,Y,Z軸周りの回転角を乱数で設定
		worldTransform.rotation_ = { rotDist(engine), rotDist(engine), rotDist(engine) };
		//X,Y,Z軸周りの平行移動（座標）を乱数で設定
		worldTransform.translation_ = { posDist(engine), posDist(engine), posDist(engine) };
		
		//単位行列を設定
		tani();

		//スケーリング倍率を行列に設定する
		matScale(worldTransform);

		//Z軸回転行列を宣言
		matRotZ(worldTransform);

		//X軸回転行列を宣言
		matRotX(worldTransform);

		//Y軸回転行列を宣言
		matRotY(worldTransform);

		//平行移動行列を設定
		matTrans(worldTransform);

		//単位行列を代入する
		worldTransform.matWorld_ = tani();

		//掛け算して代入
		//各軸の回転行列を合成
		Matrix4 matRot;

		matRot = matRotZ(worldTransform) *= matRotX(worldTransform) *= matRotY(worldTransform);

		//行列の合成
		worldTransform.matWorld_ *= matScale(worldTransform) *= matRot *= matTrans(worldTransform);

		//行列の転送
		worldTransform.TransferMatrix();
	}

	//	カメラ視点座標を設定
	viewProjection_.eye = {0,0,-50};
	
	//カメラ注視点座標の設定
	viewProjection_.target = { 10, 0, 0 };

	//カメラ上方向ベクトルを設定（右上45度指定）
	viewProjection_.up = { cosf(Rad(45)), sinf(Rad(45)), 0.0f };

	//	ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() 
{
	debugCamera_->Update();
	//視点の移動処理
	{
		//視点の移動ベクトル
		Vector3 move = { 0, 0, 0 };
		//視点の速さベクトル
		const float kEyeSpeed = 0.2f;

		//移動ベクトルの変更
		if (input_->PushKey(DIK_W)) {
			move = { 0, 0, kEyeSpeed };
		}
		else if (input_->PushKey(DIK_S)) {
			move = { 0, 0, -kEyeSpeed };
		}

		//視点移動（ベクトルの加算）
		viewProjection_.eye += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	{
		//視点の移動ベクトル
		Vector3 move = { 0, 0, 0 };
		//注視点の移動速さ
		const float kTargetSpeed = 0.2f;

		//移動ベクトル変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kTargetSpeed, 0, 0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kTargetSpeed, 0, 0 };
		}

		//注視点移動（ベクトル加算）
		viewProjection_.target += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	}

	//上方向の回転処理
	{
		//上方向の回転速さ
		const float kUpRotSpeed = 0.05f;

		//移動ベクトル変更
		if (input_->PushKey(DIK_SPACE))
		{
			viewAngle += kUpRotSpeed;
			//２π超えたら０に戻す
			viewAngle = fmodf(viewAngle, PAI * 2);
		}

		//上方向ベクトルを計算
		viewProjection_.up = { cosf(viewAngle),sinf(viewAngle), 0.0f };

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	}
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

		//範囲forですべてのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
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
