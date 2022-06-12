#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <random>
#include "Utility.h"

void Determinant(WorldTransform worldtransform_,Vector3 scale, Vector3 rotation, Vector3 translation);

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

	//乱数シード生成器
	std::random_device seed_gen;

	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 Engine(seed_gen());

	//配列で全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldtransform : worldtransforms_) {
		//ワールドトランスフォームの初期化
		worldtransform.Initialize();

			//乱数範囲の指定
		std::uniform_real_distribution<float> angleX(0, 360);
		std::uniform_real_distribution<float> angleY(0, 360);
		std::uniform_real_distribution<float> angleZ(0, 360);

		std::uniform_real_distribution<float> transX(-10, 10);
		std::uniform_real_distribution<float> transY(-10, 10);
		std::uniform_real_distribution<float> transZ(-10, 10);

		Vector3 rota = { angleX(Engine),angleY(Engine) ,angleZ(Engine) };

		Vector3 translate = { transX(Engine),transY(Engine),transZ(Engine) };

		Determinant(worldtransform, { 1,1,1 }, rota, translate);
	}
	viewProjection_.Initialize();



	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

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
	for (WorldTransform& worldtransform : worldtransforms_) {
		model_->Draw(worldtransform, debugCamera_->GetViewProjection(), textureHandle_);
	}

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

void Determinant(WorldTransform worldtransform_,Vector3 scale, Vector3 rotation, Vector3 translation) {
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