#include "PlayerBullet.h"
#include <cassert>
#include "Utility.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update(){
	Determinant(worldTransform_, scale, rotate, move);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection){
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}