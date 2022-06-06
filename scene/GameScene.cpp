#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Utility.h"

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
	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//x,y,z方向のスケーリングを設定
	worldtransform_.scale_ = { 5,5,5 };

	//スケーリング倍率を行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = worldtransform_.scale_.x;
	matScale.m[1][1] = worldtransform_.scale_.y;
	matScale.m[2][2] = worldtransform_.scale_.z;
	matScale.m[3][3] = 1;

	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_.operator*=(matScale);

	////行列の転送
	//worldtransform_.TransferMatrix();

	////x,y,z軸周りの回転角を設定
	//worldtransform_.rotation_ = { 0.0f,0.0f,radian(45.0f)};

	////z軸回転行列を宣言
	//Matrix4 matRotZ;

	//matRotZ.m[0][0] = cos(radian(45.0f));
	//matRotZ.m[0][1] = sin(radian(45.0f));
	//matRotZ.m[1][0] = -sin(radian(45.0f));
	//matRotZ.m[1][1] = cos(radian(45.0f));
	//matRotZ.m[2][2] = 1;
	//matRotZ.m[3][3] = 1;

	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_ *= matRotZ;

	////行列の転送
	//worldtransform_.TransferMatrix();

	////x,y,z軸周りの回転角を設定
	//worldtransform_.rotation_ = { radian(45.0f),0.0f,0.0f };

	////x軸回転行列を宣言
	//Matrix4 matRotX;

	//matRotX.m[0][0] = 1;
	//matRotX.m[1][1] = cos(radian(45.0f));
	//matRotX.m[1][2] = sin(radian(45.0f));
	//matRotX.m[2][1] = -sin(radian(45.0f));
	//matRotX.m[2][2] = cos(radian(45.0f));
	//matRotX.m[3][3] = 1;

	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_.operator*=(matRotX);

	////行列の転送
	//worldtransform_.TransferMatrix();

	////x,y,z軸周りの回転角を設定
	//worldtransform_.rotation_ = { 0.0f,radian(45.0f),0.0f };

	////y軸回転行列を宣言
	//Matrix4 matRotY;

	//matRotY.m[0][0] = cos(radian(45.0f));
	//matRotY.m[0][2] = -sin(radian(45.0f));
	//matRotY.m[1][1] = 1;
	//matRotY.m[2][0] = sin(radian(45.0f));
	//matRotY.m[2][2] = cos(radian(45.0f));
	//matRotY.m[3][3] = 1;

	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_.operator*=(matRotY);

	////行列の転送
	//worldtransform_.TransferMatrix();

	//x,y,z軸周りの平行移動を設定
	worldtransform_.translation_ = { 10, 10, 10 };

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldtransform_.translation_.x;
	matTrans.m[3][1] = worldtransform_.translation_.y;
	matTrans.m[3][2] = worldtransform_.translation_.z;

	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_.operator*=(matTrans);

	////行列の転送
	//worldtransform_.TransferMatrix();

	////行列の合成
	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_ = matScale * matRotZ * matRotX * matRotY * matTrans;

	//worldtransform_.TransferMatrix();

	////x,y,z軸周りの回転角を設定
	worldtransform_.rotation_ = { radian(45.0f), radian(45.0f), radian(0.0f) };

	////合成用回転行列を宣言
	Matrix4 matRot;

	////各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	//回転行列の各要素を設定する
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(radian(45.0f));
	matRotX.m[1][2] = sin(radian(45.0f));
	matRotX.m[2][1] = -sin(radian(45.0f));
	matRotX.m[2][2] = cos(radian(45.0f));
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(radian(45.0f));
	matRotY.m[0][2] = -sin(radian(45.0f));
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(radian(45.0f));
	matRotY.m[2][2] = cos(radian(45.0f));
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(radian(0.0f));
	matRotZ.m[0][1] = sin(radian(0.0f));
	matRotZ.m[1][0] = -sin(radian(0.0f));
	matRotZ.m[1][1] = cos(radian(0.0f));
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//各軸の回転行列を合成
	Matrix4 mat = matRotZ.operator*=(matRotX);
	matRot = matRotZ.operator*=(matRotY);

	//worldtransform_.matWorld_.m[0][0] = 1;
	//worldtransform_.matWorld_.m[1][1] = 1;
	//worldtransform_.matWorld_.m[2][2] = 1;
	//worldtransform_.matWorld_.m[3][3] = 1;

	//worldtransform_.matWorld_ *= matRot;

	//行列の合成
	worldtransform_.matWorld_.m[0][0] = 1;
	worldtransform_.matWorld_.m[1][1] = 1;
	worldtransform_.matWorld_.m[2][2] = 1;
	worldtransform_.matWorld_.m[3][3] = 1;

	worldtransform_.matWorld_ = matScale.operator*=(matRot).operator*=(matTrans);

	//行列の転送
	worldtransform_.TransferMatrix();
}

void GameScene::Update() {
	debugCamera_->Update();
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
