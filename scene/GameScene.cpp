#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Utility.h"

void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation);

Vector3 cross(Vector3 a, Vector3 b);

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
}

void GameScene::Update() {
	debugCamera_->Update();

	if (mode == 1) {
		float x = sin(radian(ry));
		float z = cos(radian(ry));

		float norm = sqrt(x * x + z * z);

		float normalizeX = (1 / norm) * x;
		float normalizeZ = (1 / norm) * z;

		if (input_->PushKey(DIK_W)) {
			tx += speed * normalizeX;
			tz += speed * normalizeZ;
		}
		else if (input_->PushKey(DIK_S)) {
			tx -= speed * normalizeX;
			tz -= speed * normalizeZ;
		}

		if (input_->PushKey(DIK_D)) {
			ry += 1;
		}
		else if (input_->PushKey(DIK_A)) {
			ry -= 1;
		}

		Determinant(worldtransform_, { sx, sy, sz }, { rx, ry, rz }, { tx, ty, tz });
	}

	if (mode == 2) {
		ViewProjection camera = debugCamera_->GetViewProjection();

		Vector3 pos = { worldtransform_.translation_.x - camera.eye.x,worldtransform_.translation_.y - camera.eye.y,worldtransform_.translation_.z - camera.eye.z };

		float norm = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

		float normalizeX = (1 / norm) * pos.x;
		float normalizeY = (1 / norm) * pos.y;
		float normalizeZ = (1 / norm) * pos.z;

		Vector3 normPos = { normalizeX,normalizeY,normalizeZ };

		Vector3 y = { 0,1,0 };

		Vector3 x = cross(normPos, y);

		Vector3 z = cross(normPos, x);

		y = cross(x, z);

		x = cross(y, z);

		z = cross(x, y);

		if (input_->PushKey(DIK_W)) {
			translation = { translation.x + speed * y.x,translation.y + speed * y.y ,translation.z + speed * y.z };

		}
		else if (input_->PushKey(DIK_S)) {
			translation = { translation.x - speed * y.x,translation.y - speed * y.y ,translation.z - speed * y.z };
		}

		if (input_->PushKey(DIK_D)) {
			translation = { translation.x + speed * x.x,translation.y + speed * x.y ,translation.z + speed * x.z };
		}
		else if (input_->PushKey(DIK_A)) {
			translation = { translation.x - speed * x.x,translation.y - speed * x.y ,translation.z - speed * x.z };
		}

		Determinant(worldtransform_, { 1,1,1 }, { rx,ry,rz }, translation);
	}

	if (input_->TriggerKey(DIK_Q)) {
		if (mode == 1) {
			mode = 2;
		}
		else {
			mode = 1;
		}
	}
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

	Vector3 cross(Vector3 a, Vector3 b) {
		return { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
	}