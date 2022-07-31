#include "Player.h"
#include <cassert>
#include "Input.h"
#include "DebugText.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	/*worldTransform_.Initialize();*/
}

void Player::Update(){

}

void Player::Draw(){

}