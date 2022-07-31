#include "Player.h"
#include <cassert>
#include "Input.h"
#include "DebugText.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	/*worldTransform_.Initialize();*/
}

void Player::Update(){

}

void Player::Draw(){

}