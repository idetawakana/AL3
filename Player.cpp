#include "Player.h"
#include <cassert>
#include "Utility.h"

//void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation);
//
//void translation_(WorldTransform worldTransform, Vector3 trans);
//
//void Rotation(WorldTransform worldtransform_, Vector3 rotation);

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	assert(textureHandle);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update(){

	move = { 0,0,0 };
	//�ړ�
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += 0.5f;
	}

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= 0.5f;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += 0.5f;
	}

	if (input_->PushKey(DIK_DOWN)) {
		move.y -= 0.5f;
	}

	worldTransform_.translation_ += move;

	//�ړ����E���W
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	debugText_->SetPos(200, 110);
	debugText_->Printf(" %f, %f, %f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	//����
	if (input_->PushKey(DIK_D)) {
		rotate.y -= 0.5f;
	}

	if (input_->PushKey(DIK_A)) {
		rotate.y += 0.5f;
	}

	if (input_->PushKey(DIK_W)) {
		rotate.x += 0.5f;
	}

	if (input_->PushKey(DIK_S)) {
		rotate.x -= 0.5f;
	}

	if (input_->PushKey(DIK_E)) {
		rotate.z -= 0.5f;
	}

	if (input_->PushKey(DIK_Q)) {
		rotate.z += 0.5f;
	}

	Determinant(worldTransform_, scale, rotate, worldTransform_.translation_);

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection viewProjection_){
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Attack(){
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g���������̌����ɍ��킹�ĉ�]������
		velocity = operator*(velocity, worldTransform_.matWorld_);

		//�e�𐶐���������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

//void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation) {
//	//x,y,z�����̃X�P�[�����O��ݒ�
//	worldtransform_.scale_ = scale;
//
//	//�X�P�[�����O�{�����s���錾
//	Matrix4 matScale;
//
//	matScale.m[0][0] = worldtransform_.scale_.x;
//	matScale.m[1][1] = worldtransform_.scale_.y;
//	matScale.m[2][2] = worldtransform_.scale_.z;
//	matScale.m[3][3] = 1;
//
//	//x,y,z������̕��s�ړ���ݒ�
//	worldtransform_.translation_ = translation;
//
//	//���s�ړ��s���錾
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	matTrans.m[3][0] = worldtransform_.translation_.x;
//	matTrans.m[3][1] = worldtransform_.translation_.y;
//	matTrans.m[3][2] = worldtransform_.translation_.z;
//
//	//x,y,z������̉�]�p��ݒ�
//	worldtransform_.rotation_ = rotation;
//
//	//�����p��]�s���錾
//	Matrix4 matRot;
//
//	//�e���p��]�s���錾
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	//��]�s��̊e�v�f��ݒ肷��
//	matRotX.m[0][0] = 1;
//	matRotX.m[1][1] = cos(radian(worldtransform_.rotation_.x));
//	matRotX.m[1][2] = sin(radian(worldtransform_.rotation_.x));
//	matRotX.m[2][1] = -sin(radian(worldtransform_.rotation_.x));
//	matRotX.m[2][2] = cos(radian(worldtransform_.rotation_.x));
//	matRotX.m[3][3] = 1;
//
//	matRotY.m[0][0] = cos(radian(worldtransform_.rotation_.y));
//	matRotY.m[0][2] = -sin(radian(worldtransform_.rotation_.y));
//	matRotY.m[1][1] = 1;
//	matRotY.m[2][0] = sin(radian(worldtransform_.rotation_.y));
//	matRotY.m[2][2] = cos(radian(worldtransform_.rotation_.y));
//	matRotY.m[3][3] = 1;
//
//	matRotZ.m[0][0] = cos(radian(worldtransform_.rotation_.z));
//	matRotZ.m[0][1] = sin(radian(worldtransform_.rotation_.z));
//	matRotZ.m[1][0] = -sin(radian(worldtransform_.rotation_.z));
//	matRotZ.m[1][1] = cos(radian(worldtransform_.rotation_.z));
//	matRotZ.m[2][2] = 1;
//	matRotZ.m[3][3] = 1;
//
//	//�e���̉�]�s�������
//	Matrix4 mat = matRotZ.operator*=(matRotX);
//	matRot = matRotZ.operator*=(matRotY);
//
//	//�s��̍���
//	worldtransform_.matWorld_.m[0][0] = 1;
//	worldtransform_.matWorld_.m[1][1] = 1;
//	worldtransform_.matWorld_.m[2][2] = 1;
//	worldtransform_.matWorld_.m[3][3] = 1;
//
//	worldtransform_.matWorld_ = matScale.operator*=(matRot).operator*=(matTrans);
//
//	worldtransform_.TransferMatrix();
//}
//
//void translation_(WorldTransform worldTransform, Vector3 trans) {
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	matTrans.m[3][0] = trans.x;
//	matTrans.m[3][1] = trans.y;
//	matTrans.m[3][2] = trans.z;
//
//	worldTransform.matWorld_ = MathUtility::Matrix4Identity();
//
//	worldTransform.matWorld_ *= matTrans;
//
//	worldTransform.TransferMatrix();
//}
//
//void Rotation(WorldTransform worldtransform_, Vector3 rotation) {
//	//x,y,z������̉�]�p��ݒ�
//	worldtransform_.rotation_ = rotation;
//
//	//�����p��]�s���錾
//	Matrix4 matRot;
//
//	//�e���p��]�s���錾
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	//��]�s��̊e�v�f��ݒ肷��
//	matRotX.m[0][0] = 1;
//	matRotX.m[1][1] = cos(radian(worldtransform_.rotation_.x));
//	matRotX.m[1][2] = sin(radian(worldtransform_.rotation_.x));
//	matRotX.m[2][1] = -sin(radian(worldtransform_.rotation_.x));
//	matRotX.m[2][2] = cos(radian(worldtransform_.rotation_.x));
//	matRotX.m[3][3] = 1;
//
//	matRotY.m[0][0] = cos(radian(worldtransform_.rotation_.y));
//	matRotY.m[0][2] = -sin(radian(worldtransform_.rotation_.y));
//	matRotY.m[1][1] = 1;
//	matRotY.m[2][0] = sin(radian(worldtransform_.rotation_.y));
//	matRotY.m[2][2] = cos(radian(worldtransform_.rotation_.y));
//	matRotY.m[3][3] = 1;
//
//	matRotZ.m[0][0] = cos(radian(worldtransform_.rotation_.z));
//	matRotZ.m[0][1] = sin(radian(worldtransform_.rotation_.z));
//	matRotZ.m[1][0] = -sin(radian(worldtransform_.rotation_.z));
//	matRotZ.m[1][1] = cos(radian(worldtransform_.rotation_.z));
//	matRotZ.m[2][2] = 1;
//	matRotZ.m[3][3] = 1;
//
//	//�e���̉�]�s�������
//	Matrix4 mat = matRotZ.operator*=(matRotX);
//	matRot = matRotZ.operator*=(matRotY);
//
//	worldtransform_.matWorld_ = MathUtility::Matrix4Identity();
//
//	worldtransform_.matWorld_ *= matRot;
//
//	worldtransform_.TransferMatrix();
//}