#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Utility.h"

//WorldTransform Determinant(Vector3 scale, Vector3 rotation, Vector3 translation);

void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation);

Matrix4 scale(Vector3 scale);

Matrix4 rotationX(float angle);

Matrix4 rotationY(float angle);

Matrix4 rotationZ(float angle);

Matrix4 translation(Vector3 trans);

WorldTransform world(Matrix4 matScale, Matrix4 matRotX, Matrix4 matRotY, Matrix4 matRotZ, Matrix4 matTrans);
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	model_ = Model::Create();

	worldtransform_.Initialize();
	trans = { 0,0,0 };
	rota = { 0,0,0 };
	scale = { 1,1,1 };

	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//worldtransform_ = Determinant({ 5,5,5 }, { 45,45,0 }, { 10,10,10 });

	//Matrix4 matScale = scale({ 5,5,5 });
	//
	//Matrix4 matRotX = rotationX(45);
	//Matrix4 matRotY = rotationY(45);
	//Matrix4 matRotZ = rotationZ(0);

	//Matrix4 matTrans = translation({ 10,10,10 });

	//worldtransform_ = world(matScale, matRotX, matRotY, matRotZ, matTrans);

	//worldtransform_.TransferMatrix();

	//Determinant(worldtransform_, { 5,5,5 }, { 45,45,0 }, { 10,10,10 });
}

void GameScene::Update() {
	debugCamera_->Update();

	if()

	Determinant(worldtransform_, scale, rota, trans);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 3Dモデル描画
	model_->Draw(worldtransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//WorldTransform Determinant(Vector3 scale, Vector3 rotation, Vector3 translation) {
//	//x,y,z方向のスケーリングを設定
//	WorldTransform worldtransform_;
//
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
//	////x,y,z軸周りの回転角を設定
//	worldtransform_.rotation_ = rotation;
//
//	////合成用回転行列を宣言
//	Matrix4 matRot;
//
//	////各軸用回転行列を宣言
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
//	return worldtransform_;
//}

void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation) {
	//x,y,z方向のスケーリングを設定
	worldtransform_.scale_ = scale;

	//スケーリング倍率を行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = worldtransform_.scale_.x;
	matScale.m[1][1] = worldtransform_.scale_.y;
	matScale.m[2][2] = worldtransform_.scale_.z;
	matScale.m[3][3] = 1;

	//x,y,z軸周りの平行移動を設定
	worldtransform_.translation_ = translation;

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldtransform_.translation_.x;
	matTrans.m[3][1] = worldtransform_.translation_.y;
	matTrans.m[3][2] = worldtransform_.translation_.z;

	////x,y,z軸周りの回転角を設定
	worldtransform_.rotation_ = rotation;

	////合成用回転行列を宣言
	Matrix4 matRot;

	////各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	//回転行列の各要素を設定する
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

	//各軸の回転行列を合成
	Matrix4 mat = matRotZ.operator*=(matRotX);
	matRot = matRotZ.operator*=(matRotY);

	//行列の合成
	worldtransform_.matWorld_.m[0][0] = 1;
	worldtransform_.matWorld_.m[1][1] = 1;
	worldtransform_.matWorld_.m[2][2] = 1;
	worldtransform_.matWorld_.m[3][3] = 1;

	worldtransform_.matWorld_ = matScale.operator*=(matRot).operator*=(matTrans);

	worldtransform_.TransferMatrix();
}

Matrix4 scale(Vector3 scale) {
	//スケーリング倍率を行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 rotationX(float angle) {
	////各軸用回転行列を宣言
	Matrix4 matRotX;

	//回転行列の各要素を設定する
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(radian(angle));
	matRotX.m[1][2] = sin(radian(angle));
	matRotX.m[2][1] = -sin(radian(angle));
	matRotX.m[2][2] = cos(radian(angle));
	matRotX.m[3][3] = 1;

	return matRotX;
}

Matrix4 rotationY(float angle) {
	Matrix4 matRotY;

	matRotY.m[0][0] = cos(radian(angle));
	matRotY.m[0][2] = -sin(radian(angle));
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(radian(angle));
	matRotY.m[2][2] = cos(radian(angle));
	matRotY.m[3][3] = 1;

	return matRotY;
}

Matrix4 rotationZ(float angle) {
	Matrix4 matRotZ;

	matRotZ.m[0][0] = cos(radian(angle));
	matRotZ.m[0][1] = sin(radian(angle));
	matRotZ.m[1][0] = -sin(radian(angle));
	matRotZ.m[1][1] = cos(radian(angle));
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	return matRotZ;
}

Matrix4 translation(Vector3 trans) {
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;

	return matTrans;
}

WorldTransform world(Matrix4 matScale, Matrix4 matRotX, Matrix4 matRotY, Matrix4 matRotZ, Matrix4 matTrans) {
	Matrix4 matRot;

	WorldTransform worldtransform_;

	//各軸の回転行列を合成
	Matrix4 mat = matRotZ.operator*=(matRotX);
	matRot = matRotZ.operator*=(matRotY);

	//行列の合成
	worldtransform_.matWorld_.m[0][0] = 1;
	worldtransform_.matWorld_.m[1][1] = 1;
	worldtransform_.matWorld_.m[2][2] = 1;
	worldtransform_.matWorld_.m[3][3] = 1;

	worldtransform_.matWorld_ = matScale.operator*=(matRot).operator*=(matTrans);

	return worldtransform_;
}