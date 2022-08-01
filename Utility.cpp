#include "Utility.h"
#include <WorldTransform.h>
#include <MathUtility.h>
#include "AxisIndicator.h"

float radian(float a) {
	return a * (3.141592 / 180);
}

float pi() {
	return 3.141592;
}

float angle(float a) {
	return (180 / 3.141592 * a);
}

void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation) {
	//x,y,z�����̃X�P�[�����O��ݒ�
	worldtransform_.scale_ = scale;

	//�X�P�[�����O�{�����s���錾
	Matrix4 matScale;

	matScale.m[0][0] = worldtransform_.scale_.x;
	matScale.m[1][1] = worldtransform_.scale_.y;
	matScale.m[2][2] = worldtransform_.scale_.z;
	matScale.m[3][3] = 1;

	//x,y,z������̕��s�ړ���ݒ�
	worldtransform_.translation_ = translation;

	//���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldtransform_.translation_.x;
	matTrans.m[3][1] = worldtransform_.translation_.y;
	matTrans.m[3][2] = worldtransform_.translation_.z;

	//x,y,z������̉�]�p��ݒ�
	worldtransform_.rotation_ = rotation;

	//�����p��]�s���錾
	Matrix4 matRot;

	//�e���p��]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	//��]�s��̊e�v�f��ݒ肷��
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(radian(worldtransform_.rotation_.x));
	matRotX.m[1][2] = sin(radian(worldtransform_.rotation_.x));
	matRotX.m[2][1] = -sin(radian(worldtransform_.rotation_.x));
	matRotX.m[2][2] = cos(radian(worldtransform_.rotation_.x));
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(radian(worldtransform_.rotation_.y));
	matRotY.m[0][2] = -sin(radian(worldtransform_.rotation_.y));
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(radian(worldtransform_.rotation_.y));
	matRotY.m[2][2] = cos(radian(worldtransform_.rotation_.y));
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(radian(worldtransform_.rotation_.z));
	matRotZ.m[0][1] = sin(radian(worldtransform_.rotation_.z));
	matRotZ.m[1][0] = -sin(radian(worldtransform_.rotation_.z));
	matRotZ.m[1][1] = cos(radian(worldtransform_.rotation_.z));
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//�e���̉�]�s�������
	Matrix4 mat = matRotZ.operator*=(matRotX);
	matRot = matRotZ.operator*=(matRotY);

	//�s��̍���
	worldtransform_.matWorld_.m[0][0] = 1;
	worldtransform_.matWorld_.m[1][1] = 1;
	worldtransform_.matWorld_.m[2][2] = 1;
	worldtransform_.matWorld_.m[3][3] = 1;

	worldtransform_.matWorld_ = matScale.operator*=(matRot).operator*=(matTrans);

	worldtransform_.TransferMatrix();
}

void translation_(WorldTransform worldTransform, Vector3 trans) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	worldTransform.matWorld_ *= matTrans;

	worldTransform.TransferMatrix();
}

void Rotation(WorldTransform worldtransform_, Vector3 rotation) {
	//x,y,z������̉�]�p��ݒ�
	worldtransform_.rotation_ = rotation;

	//�����p��]�s���錾
	Matrix4 matRot;

	//�e���p��]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	//��]�s��̊e�v�f��ݒ肷��
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(radian(worldtransform_.rotation_.x));
	matRotX.m[1][2] = sin(radian(worldtransform_.rotation_.x));
	matRotX.m[2][1] = -sin(radian(worldtransform_.rotation_.x));
	matRotX.m[2][2] = cos(radian(worldtransform_.rotation_.x));
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(radian(worldtransform_.rotation_.y));
	matRotY.m[0][2] = -sin(radian(worldtransform_.rotation_.y));
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(radian(worldtransform_.rotation_.y));
	matRotY.m[2][2] = cos(radian(worldtransform_.rotation_.y));
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(radian(worldtransform_.rotation_.z));
	matRotZ.m[0][1] = sin(radian(worldtransform_.rotation_.z));
	matRotZ.m[1][0] = -sin(radian(worldtransform_.rotation_.z));
	matRotZ.m[1][1] = cos(radian(worldtransform_.rotation_.z));
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//�e���̉�]�s�������
	Matrix4 mat = matRotZ.operator*=(matRotX);
	matRot = matRotZ.operator*=(matRotY);

	worldtransform_.matWorld_ = MathUtility::Matrix4Identity();

	worldtransform_.matWorld_ *= matRot;

	worldtransform_.TransferMatrix();
}

Vector3 operator*(const Vector3& v, Matrix4& m) {
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3];

	Vector3 result{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};
	return result;
}