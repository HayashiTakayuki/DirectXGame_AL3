#pragma once
#include "WorldTransform.h"
#include<cmath>

class Matrix
{
public:
	//度数法を弧度法にする関数
	float Rad(float dosu);

	//スケーリング行列を宣言
	Matrix4 matScale(WorldTransform worldTransform_);

	//Z軸回転行列を宣言
	Matrix4 matRotZ(WorldTransform worldTransform_);
	
	//X軸回転行列を宣言
	Matrix4 matRotX(WorldTransform worldTransform_);

	//Y軸回転行列を宣言
	Matrix4 matRotY(WorldTransform worldTransform_);

	//平行移動行列を設定
	Matrix4 matTrans(WorldTransform worldTransform_);

	//単位行列を設定
	Matrix4 tani();

	//行列の設定
	Matrix4 matrix(WorldTransform worldTransform_);

};
