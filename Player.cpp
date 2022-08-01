#include "Player.h"
#include <cassert>
#include "Utility.h"

//void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation);
//
//void translation_(WorldTransform worldTransform, Vector3 trans);
//
//void Rotation(WorldTransform worldtransform_, Vector3 rotation);

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	assert(textureHandle);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update(){

	move = { 0,0,0 };
	//移動
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

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	debugText_->SetPos(200, 110);
	debugText_->Printf(" %f, %f, %f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	//旋回
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

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection viewProjection_){
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Attack(){
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを時期の向きに合わせて回転させる
		velocity = operator*(velocity, worldTransform_.matWorld_);

		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

//void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation) {
//	//x,y,z方向のスケーリングを設定
//	worldtransform_.scale_ = scale;
//
//	//スケーリング倍率を行列を宣言
//	Matrix4 matScale;
//
//	matScale.m[0][0] = worldtransform_.scale_.x;
//	matScale.m[1][1] = worldtransform_.scale_.y;
//	matScale.m[2][2] = worldtransform_.scale_.z;
//	matScale.m[3][3] = 1;
//
//	//x,y,z軸周りの平行移動を設定
//	worldtransform_.translation_ = translation;
//
//	//平行移動行列を宣言
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	matTrans.m[3][0] = worldtransform_.translation_.x;
//	matTrans.m[3][1] = worldtransform_.translation_.y;
//	matTrans.m[3][2] = worldtransform_.translation_.z;
//
//	//x,y,z軸周りの回転角を設定
//	worldtransform_.rotation_ = rotation;
//
//	//合成用回転行列を宣言
//	Matrix4 matRot;
//
//	//各軸用回転行列を宣言
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	//回転行列の各要素を設定する
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
//	//各軸の回転行列を合成
//	Matrix4 mat = matRotZ.operator*=(matRotX);
//	matRot = matRotZ.operator*=(matRotY);
//
//	//行列の合成
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
//	//x,y,z軸周りの回転角を設定
//	worldtransform_.rotation_ = rotation;
//
//	//合成用回転行列を宣言
//	Matrix4 matRot;
//
//	//各軸用回転行列を宣言
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	//回転行列の各要素を設定する
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
//	//各軸の回転行列を合成
//	Matrix4 mat = matRotZ.operator*=(matRotX);
//	matRot = matRotZ.operator*=(matRotY);
//
//	worldtransform_.matWorld_ = MathUtility::Matrix4Identity();
//
//	worldtransform_.matWorld_ *= matRot;
//
//	worldtransform_.TransferMatrix();
//}