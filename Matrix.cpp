#include "Matrix.h"

//度数法を弧度法にする関数
float Matrix::Rad(float dosu)
{
	float PAI = 3.14159265358979323f;

	return dosu / 180.0f * PAI;
}

//スケーリング行列を宣言
Matrix4 Matrix::matScale(WorldTransform worldTransform_)
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
Matrix4 Matrix::matRotZ(WorldTransform worldTransform_)
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
Matrix4 Matrix::matRotX(WorldTransform worldTransform_)
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
Matrix4 Matrix::matRotY(WorldTransform worldTransform_)
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
Matrix4 Matrix::matTrans(WorldTransform worldTransform_)
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
Matrix4 Matrix::tani()
{
	Matrix4 tani = {};
	tani.m[0][0] = 1;
	tani.m[1][1] = 1;
	tani.m[2][2] = 1;
	tani.m[3][3] = 1;
	return tani;
}

//行列の設定
Matrix4 Matrix::matrix(WorldTransform worldTransform_)
{
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//単位行列を設定
	tani();

	//スケーリング倍率を行列に設定する
	matScale(worldTransform_);

	//Z軸回転行列を宣言
	matRotZ(worldTransform_);

	//X軸回転行列を宣言
	matRotX(worldTransform_);

	//Y軸回転行列を宣言
	matRotY(worldTransform_);

	//平行移動行列を設定
	matTrans(worldTransform_);

	//単位行列を代入する
	worldTransform_.matWorld_ = tani();

	//掛け算して代入
	//各軸の回転行列を合成
	Matrix4 matRot;

	matRot = matRotZ(worldTransform_) *= matRotX(worldTransform_) *= matRotY(worldTransform_);

	//行列の合成
	worldTransform_.matWorld_ *= matScale(worldTransform_) *= matRot *= matTrans(worldTransform_);

	return worldTransform_.matWorld_;
}

Vector3 Matrix::VecToMat(Vector3 vec, Matrix4 mat)
{
	Vector3 world;

	world.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + 0 * mat.m[3][0];
	world.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + 0 * mat.m[3][1];
	world.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + 0 * mat.m[3][2];

	return world;
}
