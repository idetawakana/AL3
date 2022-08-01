#include "PlayerBullet.h"
#include <cassert>
#include "Utility.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update(){	
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	Determinant(worldTransform_, scale, rotate, worldTransform_.translation_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection){
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}