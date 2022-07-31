#include "Player.h"
#include <cassert>

void translation_(WorldTransform worldTransform, Vector3 trans);

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update(){
	Vector3 move = { 0,0,0 };

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

	translation_(worldTransform_, worldTransform_.translation_);

	debugText_->SetPos(200, 110);
	debugText_->Printf(" %f, %f, %f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection_){
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
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
