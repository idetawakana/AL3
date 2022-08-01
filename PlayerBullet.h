#pragma once
#include "Model.h"
#include "WorldTransform.h"

///<summary>
///���L�����̒e
///</summary>
class PlayerBullet {
public:
	///<summary>
	///������
	///</summary>
	///<param name = "model">���f��</param>
	///<param name = "position">�������W</param>
	///<param name = "velocity">���x</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	///<summary>
	///�X�V
	///</summary>
	void Update();

	///<summary>
	///������
	///</summary>
	///<param name = "viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	Vector3 move = { 0,0,0 };
	Vector3 rotate = { 0,0,0 };
	Vector3 scale = { 1,1,1 };

	//���x
	Vector3 velocity_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};