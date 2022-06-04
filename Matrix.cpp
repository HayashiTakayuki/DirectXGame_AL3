#include "Matrix.h"

//�x���@���ʓx�@�ɂ���֐�
float Matrix::Rad(float dosu)
{
	float PAI = 3.14159265358979323f;

	return dosu / 180.0f * PAI;
}

//�X�P�[�����O�s���錾
Matrix4 Matrix::matScale(WorldTransform worldTransform_)
{
	Matrix4 matScale;
	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;
	return matScale;
}

//Z����]�s���錾
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

//X����]�s���錾
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

//Y����]�s���錾
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

//���s�ړ��s���ݒ�
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

//�P�ʍs���ݒ�
Matrix4 Matrix::tani()
{
	Matrix4 tani = {};
	tani.m[0][0] = 1;
	tani.m[1][1] = 1;
	tani.m[2][2] = 1;
	tani.m[3][3] = 1;
	return tani;
}

//�s��̐ݒ�
Matrix4 Matrix::matrix(WorldTransform worldTransform_)
{
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�P�ʍs���ݒ�
	tani();

	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale(worldTransform_);

	//Z����]�s���錾
	matRotZ(worldTransform_);

	//X����]�s���錾
	matRotX(worldTransform_);

	//Y����]�s���錾
	matRotY(worldTransform_);

	//���s�ړ��s���ݒ�
	matTrans(worldTransform_);

	//�P�ʍs���������
	worldTransform_.matWorld_ = tani();

	//�|���Z���đ��
	//�e���̉�]�s�������
	Matrix4 matRot;

	matRot = matRotZ(worldTransform_) *= matRotX(worldTransform_) *= matRotY(worldTransform_);

	//�s��̍���
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
