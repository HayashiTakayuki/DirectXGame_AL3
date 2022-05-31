#pragma once
#include "WorldTransform.h"
#include<cmath>

class Matrix
{
public:
	//�x���@���ʓx�@�ɂ���֐�
	float Rad(float dosu);

	//�X�P�[�����O�s���錾
	Matrix4 matScale(WorldTransform worldTransform_);

	//Z����]�s���錾
	Matrix4 matRotZ(WorldTransform worldTransform_);
	
	//X����]�s���錾
	Matrix4 matRotX(WorldTransform worldTransform_);

	//Y����]�s���錾
	Matrix4 matRotY(WorldTransform worldTransform_);

	//���s�ړ��s���ݒ�
	Matrix4 matTrans(WorldTransform worldTransform_);

	//�P�ʍs���ݒ�
	Matrix4 tani();

	//�s��̐ݒ�
	Matrix4 matrix(WorldTransform worldTransform_);

};
